/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "src/php_cryptopp.h"
#include "src/exception/php_exception.h"
#include "src/padding/php_padding_interface.h"
#include "src/padding/php_pkcs7.h"
#include "src/symmetric/cipher/php_symmetric_transformation_interface.h"
#include "src/symmetric/cipher/authenticated/php_authenticated_symmetric_cipher_interface.h"
#include "src/symmetric/cipher/stream/php_stream_cipher_abstract.h"
#include "src/symmetric/cipher/symmetric_transformation_proxy.h"
#include "src/symmetric/mode/php_symmetric_mode_abstract.h"
#include "src/utils/zend_object_utils.h"
#include "src/utils/zval_utils.h"
#include "php_symmetric_transformation_filter.h"
#include <exception>
#include <filters.h>
#include <modes.h>
#include <zend_exceptions.h>

/* {{{ fork of CryptoPP::StreamTransformationFilter to support padding schemes as objects */
SymmetricTransformationFilter::SymmetricTransformationFilter(CryptoPP::StreamTransformation &cipher, zval *paddingObject, bool cipherMustBeDestructed TSRMLS_DC, bool allowAuthenticatedSymmetricCipher)
    : CryptoPP::FilterWithBufferedInput(NULL)
    , m_cipher(cipher)
    , m_optimalBufferSize(0)
    , m_cipherMustBeDestructed(cipherMustBeDestructed)
{
    SET_M_TSRMLS_C()

    // check paddingObject
    if (NULL == paddingObject || !instanceof_function(Z_OBJCE_P(paddingObject), cryptopp_ce_PaddingInterface TSRMLS_CC)) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp internal error: expected instance of PaddingInterface");
        throw false;
    }

    m_paddingObject = paddingObject;

    // initialization copied from CryptoPP::StreamTransformationFilter
    assert(cipher.MinLastBlockSize() == 0 || cipher.MinLastBlockSize() > cipher.MandatoryBlockSize());
    IsolatedInitialize(MakeParameters(CryptoPP::Name::BlockPaddingScheme(), NO_PADDING, false));
}

SymmetricTransformationFilter::~SymmetricTransformationFilter()
{
    if (m_cipherMustBeDestructed) {
        delete &m_cipher;
    }
}

bool SymmetricTransformationFilter::PaddingObjectCanUnpad()
{
    zval *funcName  = makeZval("canUnpad");
    zval *canUnpad  = call_user_method(m_paddingObject, funcName M_TSRMLS_CC);
    bool result     = false;

    if (IS_BOOL == Z_TYPE_P(canUnpad) && 1 == Z_BVAL_P(canUnpad)) {
        result = true;
    }

    zval_ptr_dtor(&canUnpad);
    zval_ptr_dtor(&funcName);
    return result;
}

void SymmetricTransformationFilter::InitializeDerivedAndReturnNewSizes(const CryptoPP::NameValuePairs &parameters, size_t &firstSize, size_t &blockSize, size_t &lastSize)
{
    firstSize = 0;
    blockSize = m_cipher.MandatoryBlockSize();

    if (m_cipher.MinLastBlockSize() > 0) {
        lastSize = m_cipher.MinLastBlockSize();
    } else if (m_cipher.MandatoryBlockSize() > 1 && !m_cipher.IsForwardTransformation() && PaddingObjectCanUnpad()) {
        lastSize = m_cipher.MandatoryBlockSize();
    } else {
        lastSize = 0;
    }
}

void SymmetricTransformationFilter::LastPut(const byte *inString, size_t length)
{
    // if the cipher is not a block cipher, no padding is needed
    bool isBlockCipher = (m_cipher.MandatoryBlockSize() > 1 && m_cipher.MinLastBlockSize() == 0);

    if (!isBlockCipher) {
        if (length > 0) {
            byte processed[length];
            m_cipher.ProcessData(processed, inString, length);
            AttachedTransformation()->Put(processed, length);
        }

        return;
    }

    // prepare padding
    unsigned int blockSize  = m_cipher.MandatoryBlockSize();
    byte *output            = NULL;

    zval *zInput            = NULL;
    zval *funcName          = NULL;
    zval *zOutput           = NULL;
    zval *zBlockSize        = makeZval(blockSize);

    try {
        if (m_cipher.IsForwardTransformation()) {
            // pad
            funcName    = makeZval("pad");
            zInput      = makeZval(reinterpret_cast<const char*>(inString), length);
            zOutput     = call_user_method(m_paddingObject, funcName, zInput, zBlockSize M_TSRMLS_CC);
            zval_ptr_dtor(&zInput);
            zInput      = NULL;

            if (IS_STRING != Z_TYPE_P(zOutput)) {
                throw false;
            }

            // ensure zOutput length is a multiple of block size
            int outputLength = Z_STRLEN_P(zOutput);

            if (0 != outputLength % blockSize) {
                zend_throw_exception_ex(getCryptoppException(), 0 M_TSRMLS_CC, (char*)"SymmetricTransformationFilter: cipher text last block length (%u) is not a multiple of block size (%u)", length, blockSize);
                throw false;
            }

            // write zOutput content
            if (outputLength > 0) {
                output = reinterpret_cast<byte*>(Z_STRVAL_P(zOutput));
                m_cipher.ProcessData(output, output, outputLength);
                AttachedTransformation()->Put(output, outputLength);
            }
        } else {
            // unpad
            // ensure inString length is a multiple of block size
            if (0 != length % blockSize) {
                zend_throw_exception_ex(getCryptoppException(), 0 M_TSRMLS_CC, (char*)"SymmetricTransformationFilter: cipher text last block length (%u) is not a multiple of block size (%u)", length, blockSize);
                throw false;
            } else if (0 == length) {
                // nothing to unpad
                return;
            }

            // unpad
            byte plain[length];
            m_cipher.ProcessData(plain, inString, length);

            funcName    = makeZval("unpad");
            zInput      = makeZval(reinterpret_cast<const char*>(plain), length);
            zOutput     = call_user_method(m_paddingObject, funcName, zInput, zBlockSize M_TSRMLS_CC);
            zval_ptr_dtor(&zInput);
            zInput      = NULL;

            if (IS_STRING != Z_TYPE_P(zOutput)) {
                throw false;
            }

            // write zOutput content
            int outputLength = Z_STRLEN_P(zOutput);

            if (outputLength > 0) {
                output = reinterpret_cast<byte*>(Z_STRVAL_P(zOutput));
                AttachedTransformation()->Put(output, outputLength);
            }
        }
    } catch (const std::exception &e) {
        // free zvals whatever happen
        zval_ptr_dtor(&zBlockSize);

        if (NULL != zInput) {
            zval_ptr_dtor(&zInput);
        }

        if (NULL != funcName) {
            zval_ptr_dtor(&funcName);
        }

        if (NULL != zOutput) {
            zval_ptr_dtor(&zOutput);
        }

        if (NULL != zInput) {
            zval_ptr_dtor(&zInput);
        }

        throw e;
    } catch (bool e) {
        // free zvals whatever happen
        zval_ptr_dtor(&zBlockSize);

        if (NULL != zInput) {
            zval_ptr_dtor(&zInput);
        }

        if (NULL != funcName) {
            zval_ptr_dtor(&funcName);
        }

        if (NULL != zOutput) {
            zval_ptr_dtor(&zOutput);
        }

        if (NULL != zInput) {
            zval_ptr_dtor(&zInput);
        }

        throw e;
    }

    // free zvals
    zval_ptr_dtor(&zBlockSize);

    if (NULL != zInput) {
        zval_ptr_dtor(&zInput);
    }

    if (NULL != funcName) {
        zval_ptr_dtor(&funcName);
    }

    if (NULL != zOutput) {
        zval_ptr_dtor(&zOutput);
    }

    if (NULL != zInput) {
        zval_ptr_dtor(&zInput);
    }
}
/* }}} */

/* {{{ copied from CryptoPP::StreamTransformationFilter */
void SymmetricTransformationFilter::FirstPut(const byte *inString)
{
    m_optimalBufferSize = m_cipher.OptimalBlockSize();
    m_optimalBufferSize = (unsigned int)CryptoPP::STDMAX(m_optimalBufferSize, CryptoPP::RoundDownToMultipleOf(4096U, m_optimalBufferSize));
}

void SymmetricTransformationFilter::NextPutMultiple(const byte *inString, size_t length)
{
    if (!length) {
        return;
    }

    size_t s = m_cipher.MandatoryBlockSize();

    do {
        size_t len  = m_optimalBufferSize;
        byte *space = HelpCreatePutSpace(*AttachedTransformation(), CryptoPP::DEFAULT_CHANNEL, s, length, len);

        if (len < length) {
            if (len == m_optimalBufferSize) {
                len -= m_cipher.GetOptimalBlockSizeUsed();
            }

            len = CryptoPP::RoundDownToMultipleOf(len, s);
        } else {
            len = length;
        }

        m_cipher.ProcessString(space, inString, len);
        AttachedTransformation()->PutModifiable(space, len);
        inString   += len;
        length     -= len;
    } while (length > 0);
}

void SymmetricTransformationFilter::NextPutModifiable(byte *inString, size_t length)
{
    m_cipher.ProcessString(inString, length);
    AttachedTransformation()->PutModifiable(inString, length);
}
/* }}} */

/* {{{ arg info */
ZEND_BEGIN_ARG_INFO_EX(arginfo_SymmetricTransformationFilter___construct, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, cipher, Cryptopp\\SymmetricTransformationInterface, 0)
    ZEND_ARG_OBJ_INFO(0, padding, Cryptopp\\PaddingInterface, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricTransformationFilter___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricTransformationFilter___sleep, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricTransformationFilter_getCipher, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricTransformationFilter_getPadding, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricTransformationFilter_encryptString, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricTransformationFilter_decryptString, 0)
    ZEND_ARG_INFO(0, ciphertext)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ SymmetricTransformationFilter_free_storage
   custom object free handler */
void SymmetricTransformationFilter_free_storage(void *object TSRMLS_DC) {
    SymmetricTransformationFilterContainer *obj = static_cast<SymmetricTransformationFilterContainer *>(object);
    delete obj->stfEncryptor;
    delete obj->stfDecryptor;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}
/* }}} */

/* {{{ PHP class declaration */
zend_object_handlers SymmetricTransformationFilter_object_handlers;
zend_class_entry *cryptopp_ce_SymmetricTransformationFilter;

static zend_function_entry cryptopp_methods_SymmetricTransformationFilter[] = {
    PHP_ME(Cryptopp_SymmetricTransformationFilter, __construct, arginfo_SymmetricTransformationFilter___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Cryptopp_SymmetricTransformationFilter, __sleep, arginfo_SymmetricTransformationFilter___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricTransformationFilter, __wakeup, arginfo_SymmetricTransformationFilter___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricTransformationFilter, getCipher, arginfo_SymmetricTransformationFilter_getCipher, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricTransformationFilter, getPadding, arginfo_SymmetricTransformationFilter_getPadding, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricTransformationFilter, encryptString, arginfo_SymmetricTransformationFilter_encryptString, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricTransformationFilter, decryptString, arginfo_SymmetricTransformationFilter_decryptString, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_SymmetricTransformationFilter(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "SymmetricTransformationFilter", cryptopp_methods_SymmetricTransformationFilter);
    cryptopp_ce_SymmetricTransformationFilter                   = zend_register_internal_class(&ce TSRMLS_CC);

    cryptopp_ce_SymmetricTransformationFilter->create_object    = zend_custom_create_handler<SymmetricTransformationFilterContainer, SymmetricTransformationFilter_free_storage, &SymmetricTransformationFilter_object_handlers>;
    memcpy(&SymmetricTransformationFilter_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    SymmetricTransformationFilter_object_handlers.clone_obj     = NULL;

    zend_declare_property_null(cryptopp_ce_SymmetricTransformationFilter, "cipher", 6, ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_null(cryptopp_ce_SymmetricTransformationFilter, "padding", 7, ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ getCryptoppSymmetricTransformationFilterEncryptorPtr
   get the pointer to the native stf encryptor object of the php class */
static SymmetricTransformationFilter *getCryptoppSymmetricTransformationFilterEncryptorPtr(zval *this_ptr TSRMLS_DC) {
    SymmetricTransformationFilter *stf = static_cast<SymmetricTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfEncryptor;

    if (NULL == stf) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\SymmetricTransformationFilter : constructor was not called");
    }

    return stf;
}
/* }}} */

/* {{{ getCryptoppSymmetricTransformationFilterDecryptorPtr
   get the pointer to the native stf decryptor object of the php class */
static SymmetricTransformationFilter *getCryptoppSymmetricTransformationFilterDecryptorPtr(zval *this_ptr TSRMLS_DC) {
    SymmetricTransformationFilter *stf = static_cast<SymmetricTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfDecryptor;

    if (NULL == stf) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\SymmetricTransformationFilter : constructor was not called");
    }

    return stf;
}
/* }}} */

/* {{{ setCryptoppSymmetricTransformationFilterEncryptorPtr
   set the pointer to the native stf encryptor object of the php class */
static void setCryptoppSymmetricTransformationFilterEncryptorPtr(zval *this_ptr, SymmetricTransformationFilter *nativePtr TSRMLS_DC) {
    static_cast<SymmetricTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfEncryptor = nativePtr;
}
/* }}} */

/* {{{ setCryptoppSymmetricTransformationFilterDecryptorPtr
   set the pointer to the native stf decryptor object of the php class */
static void setCryptoppSymmetricTransformationFilterDecryptorPtr(zval *this_ptr, SymmetricTransformationFilter *nativePtr TSRMLS_DC) {
    static_cast<SymmetricTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfDecryptor = nativePtr;
}
/* }}} */

/* {{{ isNativeSymmetricTransformationObjectValid
   indicates if the native symmetric transformation object holded by a stf object is valid */
static bool isNativeSymmetricTransformationObjectValid(zval *stfObject TSRMLS_DC) {
    zval *cipherObject = zend_read_property(cryptopp_ce_SymmetricTransformationFilter, stfObject, "cipher", 6, 0 TSRMLS_CC);

    if (IS_OBJECT != Z_TYPE_P(cipherObject)) {
        // not an object
        return false;
    } else if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_SymmetricModeAbstract TSRMLS_CC)) {
        // SymmetricModeAbstract
        CryptoPP::SymmetricCipher *modeEncryptor = getCryptoppSymmetricModeEncryptorPtr(cipherObject TSRMLS_CC);

        if (!isCryptoppSymmetricModeKeyValid(cipherObject, modeEncryptor TSRMLS_CC) || !isCryptoppSymmetricModeIvValid(cipherObject, modeEncryptor TSRMLS_CC)) {
            return false;
        }
    } else if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_StreamCipherAbstract TSRMLS_CC)) {
        // StreamCipherAbstract
        CryptoPP::SymmetricCipher *streamCipherEncryptor = getCryptoppSymmetricModeEncryptorPtr(cipherObject TSRMLS_CC);

        if (!isCryptoppStreamCipherKeyValid(cipherObject, streamCipherEncryptor TSRMLS_CC) || !isCryptoppStreamCipherIvValid(cipherObject, streamCipherEncryptor TSRMLS_CC)) {
            return false;
        }
    }

    return true;
}
/* }}} */

/* {{{ restartSymmetricCipherObject
   restart the cipher holded by a SymmetricTransformationFilter php object */
static void restartSymmetricCipherObject(zval *stfObject TSRMLS_DC) {
    zval *cipherObject  = zend_read_property(cryptopp_ce_SymmetricTransformationFilter, stfObject, "cipher", 6, 0 TSRMLS_CC);
    zval *funcName      = makeZval("restart");
    zval *output        = call_user_method(cipherObject, funcName TSRMLS_CC);
    zval_ptr_dtor(&funcName);
    zval_ptr_dtor(&output);
}
/* }}} */

/* {{{ proto void SymmetricTransformationFilter::__sleep(void)
   Prevents serialization of a SymmetricTransformationFilter instance */
PHP_METHOD(Cryptopp_SymmetricTransformationFilter, __sleep) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\SymmetricTransformationFilter instances");
}
/* }}} */

/* {{{ proto void SymmetricTransformationFilter::__wakeup(void)
   Prevents use of a SymmetricTransformationFilter instance that has been unserialized */
PHP_METHOD(Cryptopp_SymmetricTransformationFilter, __wakeup) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\SymmetricTransformationFilter instances");
}
/* }}} */

/* {{{ proto SymmetricTransformationFilter::__construct(Cryptopp\SymmetricTransformationInterface cipher [, Cryptopp\PaddingInterface padding]) */
PHP_METHOD(Cryptopp_SymmetricTransformationFilter, __construct) {
    zval *cipherObject;
    zval *paddingObject = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O|O", &cipherObject, cryptopp_ce_SymmetricTransformationInterface, &paddingObject, cryptopp_ce_PaddingInterface)) {
        return;
    }

    // produce a notice if the cipher object is an authenticated cipher
    if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_AuthenticatedSymmetricCipherInterface TSRMLS_CC)) {
        php_error_docref(NULL TSRMLS_CC, E_NOTICE, "SymmetricTransformationFilter: instances of AuthenticatedSymmetricCipher should be used with AuthenticatedSymmetricTransformationFilter");
    }

    // if no padding object, pick the default one
    bool createdPadding = false;

    if (NULL == paddingObject) {
        MAKE_STD_ZVAL(paddingObject);
        object_init_ex(paddingObject, cryptopp_ce_PaddingPkcs7);
        createdPadding = true;
    }

    // retrieve native cipher object
    CryptoPP::SymmetricCipher *symmetricEncryptor;
    CryptoPP::SymmetricCipher *symmetricDecryptor;
    bool parentConstructorError = false;
    bool cipherMustBeDestructed = false;

    if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_SymmetricModeAbstract TSRMLS_CC)) {
        // retrieve native mode objects
        symmetricEncryptor = getCryptoppSymmetricModeEncryptorPtr(cipherObject TSRMLS_CC);
        symmetricDecryptor = getCryptoppSymmetricModeDecryptorPtr(cipherObject TSRMLS_CC);

        if (NULL == symmetricEncryptor || NULL == symmetricDecryptor) {
            parentConstructorError = true;
        }
    } else if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_StreamCipherAbstract TSRMLS_CC)) {
        // retrieve native stream cipher objects
        symmetricEncryptor = getCryptoppStreamCipherEncryptorPtr(cipherObject TSRMLS_CC);
        symmetricDecryptor = getCryptoppStreamCipherDecryptorPtr(cipherObject TSRMLS_CC);

        if (NULL == symmetricEncryptor || NULL == symmetricDecryptor) {
            parentConstructorError = true;
        }
    } else {
        // create a proxy to the user php object
        try {
            symmetricEncryptor      = new SymmetricTransformationProxy::Encryption(cipherObject TSRMLS_CC);
            symmetricDecryptor      = new SymmetricTransformationProxy::Decryption(cipherObject TSRMLS_CC);
            cipherMustBeDestructed  = true;
        } catch (bool e) {
            return;
        } catch (const char *e) {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp internal error: SymmetricTransformationFilter: %s", e);

            if (createdPadding) {
                zval_ptr_dtor(&paddingObject);
            }

            return;
        }
    }

    if (parentConstructorError) {
        zend_class_entry *ce = zend_get_class_entry(cipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : parent constructor was not called", ce->name);

        if (createdPadding) {
            zval_ptr_dtor(&paddingObject);
        }

        return;
    }

    // create native symmetric transformation filter
    SymmetricTransformationFilter *stfEncryptor;
    SymmetricTransformationFilter *stfDecryptor;

    try {
        stfEncryptor = new SymmetricTransformationFilter(*symmetricEncryptor, paddingObject, cipherMustBeDestructed TSRMLS_CC);
        stfDecryptor = new SymmetricTransformationFilter(*symmetricDecryptor, paddingObject, cipherMustBeDestructed TSRMLS_CC);
    } catch (std::exception &e) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp internal error: SymmetricTransformationFilter: %s", e.what());

        if (createdPadding) {
            zval_ptr_dtor(&paddingObject);
        }

        return;
    }

    setCryptoppSymmetricTransformationFilterEncryptorPtr(getThis(), stfEncryptor TSRMLS_CC);
    setCryptoppSymmetricTransformationFilterDecryptorPtr(getThis(), stfDecryptor TSRMLS_CC);

    // hold the cipher mode object. if not, it can be deleted and associated encryptor/decryptor objects will be deleted too
    // same for padding
    zend_update_property(cryptopp_ce_SymmetricTransformationFilter, getThis(), "cipher", 6, cipherObject TSRMLS_CC);
    zend_update_property(cryptopp_ce_SymmetricTransformationFilter, getThis(), "padding", 7, paddingObject TSRMLS_CC);

    if (createdPadding) {
        zval_ptr_dtor(&paddingObject);
    }
}
/* }}} */

/* {{{ proto Cryptopp\SymmetricTransformationInterface SymmetricTransformationFilter::getCipher(void)
   Returns the cipher object */
PHP_METHOD(Cryptopp_SymmetricTransformationFilter, getCipher) {
    zval *cipherObject = zend_read_property(cryptopp_ce_SymmetricTransformationFilter, getThis(), "cipher", 6, 0 TSRMLS_CC);
    RETURN_ZVAL(cipherObject, 1, 0)
}
/* }}} */

/* {{{ proto Cryptopp\PaddingInterface SymmetricTransformationFilter::getPadding(void)
   Returns the padding object */
PHP_METHOD(Cryptopp_SymmetricTransformationFilter, getPadding) {
    zval *paddingObject = zend_read_property(cryptopp_ce_SymmetricTransformationFilter, getThis(), "padding", 7, 0 TSRMLS_CC);
    RETURN_ZVAL(paddingObject, 1, 0)
}
/* }}} */

// TODO method encrypt/decrypt: accept an input and output object (return boolean)

/* {{{ proto bool|string SymmetricTransformationFilter::encryptString(string data)
   Encrypts a string */
PHP_METHOD(Cryptopp_SymmetricTransformationFilter, encryptString) {
    char *data      = NULL;
    int dataSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &dataSize)) {
        return;
    }

    SymmetricTransformationFilter *stfEncryptor;
    CRYPTOPP_SYMMETRIC_TRANSFORMATION_FILTER_GET_ENCRYPTOR_PTR(stfEncryptor)

    // if the mode object is a native object, ensure that the key/iv is valid
    if (!isNativeSymmetricTransformationObjectValid(getThis() TSRMLS_CC)) {
        RETURN_FALSE
    }

    // encrypt
    try {
        stfEncryptor->GetNextMessage();
        stfEncryptor->Put(reinterpret_cast<byte*>(data), dataSize);
        stfEncryptor->MessageEnd();

        CryptoPP::lword retrievable = stfEncryptor->MaxRetrievable();
        restartSymmetricCipherObject(getThis() TSRMLS_CC);

        if (retrievable >= dataSize) {
            // return ciphertext
            byte *ciphertext = new byte[retrievable];
            stfEncryptor->Get(ciphertext, retrievable);
            RETVAL_STRINGL(reinterpret_cast<char*>(ciphertext), retrievable, 1);
            delete[] ciphertext;

            return;
        } else {
            // something goes wrong
            RETURN_FALSE
        }
    } catch (bool e) {
        RETURN_FALSE
    }
}
/* }}} */

/* {{{ proto bool|string SymmetricTransformationFilter::decryptString(string ciphertext)
   Decrypts a string */
PHP_METHOD(Cryptopp_SymmetricTransformationFilter, decryptString) {
    char *ciphertext    = NULL;
    int ciphertextSize  = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ciphertext, &ciphertextSize)) {
        return;
    }

    SymmetricTransformationFilter *stfDecryptor;
    CRYPTOPP_SYMMETRIC_TRANSFORMATION_FILTER_GET_DECRYPTOR_PTR(stfDecryptor)

    // if the mode object is a native object, ensure that the key/iv is valid
    if (!isNativeSymmetricTransformationObjectValid(getThis() TSRMLS_CC)) {
        RETURN_FALSE
    }

    // decrypt
    try {
        stfDecryptor->GetNextMessage();
        size_t written = stfDecryptor->Put(reinterpret_cast<byte*>(ciphertext), ciphertextSize);
        stfDecryptor->MessageEnd();

        CryptoPP::lword retrievable = stfDecryptor->MaxRetrievable();
        restartSymmetricCipherObject(getThis() TSRMLS_CC);

        if (retrievable > 0 && retrievable <= ciphertextSize) {
            // return plain text
            byte *plaintext = new byte[retrievable];
            stfDecryptor->Get(plaintext, retrievable);
            RETVAL_STRINGL(reinterpret_cast<char*>(plaintext), retrievable, 1);
            delete[] plaintext;

            return;
        } else {
            // something goes wrong
            RETURN_FALSE
        }
    } catch (bool e) {
        RETURN_FALSE
    }
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
