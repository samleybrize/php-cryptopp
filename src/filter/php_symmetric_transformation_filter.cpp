#include "../php_cryptopp.h"
#include "../exception/php_exception.h"
#include "../padding/php_padding_interface.h"
#include "../padding/php_pkcs7.h"
#include "../symmetric/cipher/php_symmetric_transformation_interface.h"
#include "../symmetric/cipher/authenticated/php_authenticated_symmetric_cipher_interface.h"
#include "../symmetric/cipher/stream/php_stream_cipher_abstract.h"
#include "../symmetric/cipher/symmetric_transformation_proxy.h"
#include "../symmetric/mode/php_symmetric_mode_abstract.h"
#include "php_symmetric_transformation_filter.h"
#include <exception>
#include <filters.h>
#include <modes.h>
#include <zend_exceptions.h>

/* {{{ fork of CryptoPP::StreamTransformationFilter to support padding schemes as objects */
SymmetricTransformationFilter::SymmetricTransformationFilter(CryptoPP::StreamTransformation &cipher, zval *paddingObject, bool cipherMustBeDestructed, bool allowAuthenticatedSymmetricCipher)
    : CryptoPP::FilterWithBufferedInput(NULL)
    , m_cipher(cipher)
    , m_optimalBufferSize(0)
    , m_cipherMustBeDestructed(cipherMustBeDestructed)
{
    // check paddingObject
    if (NULL == paddingObject || !instanceof_function(Z_OBJCE_P(paddingObject), cryptopp_ce_PaddingInterface)) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp internal error: expected instance of PaddingInterface");
        throw false;
    }

    m_paddingObject = paddingObject;

    // initialization copied from CryptoPP::StreamTransformationFilter
    assert(c.MinLastBlockSize() == 0 || c.MinLastBlockSize() > c.MandatoryBlockSize());
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
    zval *canUnpad;
    zval *funcName;
    MAKE_STD_ZVAL(canUnpad);
    MAKE_STD_ZVAL(funcName);
    ZVAL_STRING(funcName, "canUnpad", 1);
    call_user_function(NULL, &m_paddingObject, funcName, canUnpad, 0, NULL TSRMLS_CC);
    bool result = false;

    if (IS_BOOL == Z_TYPE_P(canUnpad) && 1 == Z_BVAL_P(canUnpad)) {
        result = true;
    }

    zval_dtor(canUnpad);
    zval_dtor(funcName);
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

    zval *funcName;
    zval *zInput;
    zval *zOutput;
    zval *zBlockSize;
    MAKE_STD_ZVAL(funcName);
    MAKE_STD_ZVAL(zInput);
    MAKE_STD_ZVAL(zOutput);
    MAKE_STD_ZVAL(zBlockSize);

    ZVAL_LONG(zBlockSize, blockSize);
    zval *params[] = {zInput, zBlockSize};

    try {
        if (m_cipher.IsForwardTransformation()) {
            // pad
            ZVAL_STRING(funcName, "pad", 1);
            ZVAL_STRINGL(zInput, reinterpret_cast<const char*>(inString), length, 1);
            call_user_function(NULL, &m_paddingObject, funcName, zOutput, 2, params TSRMLS_CC);
            Z_DELREF_P(zInput);
            zInput = NULL;

            if (IS_STRING != Z_TYPE_P(zOutput)) {
                throw false;
            }

            // ensure zOutput length is a multiple of block size
            int outputLength = Z_STRLEN_P(zOutput);

            if (0 != outputLength % blockSize) {
                zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"SymmetricTransformationFilter: cipher text last block length (%u) is not a multiple of block size (%u)", length, blockSize);
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
                zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"SymmetricTransformationFilter: cipher text last block length (%u) is not a multiple of block size (%u)", length, blockSize);
                throw false;
            } else if (0 == length) {
                // nothing to unpad
                return;
            }

            // unpad
            byte plain[length];
            m_cipher.ProcessData(plain, inString, length);

            ZVAL_STRING(funcName, "unpad", 1);
            ZVAL_STRINGL(zInput, reinterpret_cast<char*>(plain), length, 1);
            call_user_function(NULL, &m_paddingObject, funcName, zOutput, 2, params TSRMLS_CC);
            Z_DELREF_P(zInput);
            zInput = NULL;

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
        zval_dtor(funcName);
        zval_dtor(zOutput);
        Z_DELREF_P(zBlockSize);

        if (NULL != zInput) {
            Z_DELREF_P(zInput);
        }

        throw e;
    }

    // free zvals
    zval_dtor(funcName);
    zval_dtor(zOutput);
    Z_DELREF_P(zBlockSize);

    if (NULL != zInput) {
        Z_DELREF_P(zInput);
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

/* {{{ custom object create/free handler */
zend_object_handlers SymmetricTransformationFilter_object_handlers;

void SymmetricTransformationFilter_free_storage(void *object TSRMLS_DC) {
    SymmetricTransformationFilterContainer *obj = static_cast<SymmetricTransformationFilterContainer *>(object);
    delete obj->stfEncryptor;
    delete obj->stfDecryptor;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}

zend_object_value SymmetricTransformationFilter_create_handler(zend_class_entry *type TSRMLS_DC) {
    zend_object_value retval;

    SymmetricTransformationFilterContainer *obj = static_cast<SymmetricTransformationFilterContainer *>(emalloc(sizeof(SymmetricTransformationFilterContainer)));
    memset(obj, 0, sizeof(SymmetricTransformationFilterContainer));

    zend_object_std_init(&obj->std, type TSRMLS_CC);

    #if PHP_VERSION_ID < 50399
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    #else
        object_properties_init(static_cast<zend_object*>(&(obj->std)), type);
    #endif

    retval.handle   = zend_objects_store_put(obj, NULL, SymmetricTransformationFilter_free_storage, NULL TSRMLS_CC);
    retval.handlers = &SymmetricTransformationFilter_object_handlers;

    return retval;
}
/* }}} */

/* {{{ PHP class declaration */
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
    cryptopp_ce_SymmetricTransformationFilter = zend_register_internal_class(&ce TSRMLS_CC);

    cryptopp_ce_SymmetricTransformationFilter->create_object = SymmetricTransformationFilter_create_handler;
    memcpy(&SymmetricTransformationFilter_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    SymmetricTransformationFilter_object_handlers.clone_obj = NULL;

    zend_declare_property_null(cryptopp_ce_SymmetricTransformationFilter, "cipher", 6, ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_null(cryptopp_ce_SymmetricTransformationFilter, "padding", 7, ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ get the pointer to the native stf encryptor object of the php class */
static SymmetricTransformationFilter *getCryptoppSymmetricTransformationFilterEncryptorPtr(zval *this_ptr TSRMLS_DC) {
    SymmetricTransformationFilter *stf;
    stf = static_cast<SymmetricTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfEncryptor;

    if (NULL == stf) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\SymmetricTransformationFilter : constructor was not called");
    }

    return stf;
}
/* }}} */

/* {{{ get the pointer to the native stf decryptor object of the php class */
static SymmetricTransformationFilter *getCryptoppSymmetricTransformationFilterDecryptorPtr(zval *this_ptr TSRMLS_DC) {
    SymmetricTransformationFilter *stf;
    stf = static_cast<SymmetricTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfDecryptor;

    if (NULL == stf) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\SymmetricTransformationFilter : constructor was not called");
    }

    return stf;
}
/* }}} */

/* {{{ set the pointer to the native stf encryptor object of the php class */
static void setCryptoppSymmetricTransformationFilterEncryptorPtr(zval *this_ptr, SymmetricTransformationFilter *nativePtr TSRMLS_DC) {
    static_cast<SymmetricTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfEncryptor = nativePtr;
}
/* }}} */

/* {{{ set the pointer to the native stf decryptor object of the php class */
static void setCryptoppSymmetricTransformationFilterDecryptorPtr(zval *this_ptr, SymmetricTransformationFilter *nativePtr TSRMLS_DC) {
    static_cast<SymmetricTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfDecryptor = nativePtr;
}
/* }}} */

/* {{{ indicates if the native symmetric transformation object holded by a stf object is valid */
static bool isNativeSymmetricTransformationObjectValid(zval *stfObject) {
    zval *cipherObject;
    cipherObject = zend_read_property(cryptopp_ce_SymmetricTransformationFilter, stfObject, "cipher", 6, 0 TSRMLS_CC);

    if (IS_OBJECT != Z_TYPE_P(cipherObject)) {
        // not an object
        return false;
    } else if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_SymmetricModeAbstract)) {
        // SymmetricModeAbstract
        CryptoPP::SymmetricCipher *modeEncryptor;
        modeEncryptor = getCryptoppSymmetricModeEncryptorPtr(cipherObject);

        if (!isCryptoppSymmetricModeKeyValid(cipherObject, modeEncryptor) || !isCryptoppSymmetricModeIvValid(cipherObject, modeEncryptor)) {
            return false;
        }
    } else if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_StreamCipherAbstract)) {
        // StreamCipherAbstract
        CryptoPP::SymmetricCipher *streamCipherEncryptor;
        streamCipherEncryptor = getCryptoppSymmetricModeEncryptorPtr(cipherObject);

        if (!isCryptoppStreamCipherKeyValid(cipherObject, streamCipherEncryptor) || !isCryptoppStreamCipherIvValid(cipherObject, streamCipherEncryptor)) {
            return false;
        }
    }

    return true;
}
/* }}} */

/* {{{ restart the cipher holded by a SymmetricTransformationFilter php object */
static void restartSymmetricCipherObject(zval *stfObject) {
    zval *cipherObject;
    cipherObject = zend_read_property(cryptopp_ce_SymmetricTransformationFilter, stfObject, "cipher", 6, 0 TSRMLS_CC);

    zval *funcName;
    MAKE_STD_ZVAL(funcName)
    ZVAL_STRING(funcName, "restart", 1);
    call_user_function(NULL, &cipherObject, funcName, funcName, 0, NULL TSRMLS_CC);
    zval_dtor(funcName);
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
    if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_AuthenticatedSymmetricCipherInterface)) {
        php_error_docref(NULL, E_NOTICE, "SymmetricTransformationFilter: instances of AuthenticatedSymmetricCipher should be used with AuthenticatedSymmetricTransformationFilter");
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

    if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_SymmetricModeAbstract)) {
        // retrieve native mode objects
        symmetricEncryptor = getCryptoppSymmetricModeEncryptorPtr(cipherObject);
        symmetricDecryptor = getCryptoppSymmetricModeDecryptorPtr(cipherObject);

        if (NULL == symmetricEncryptor || NULL == symmetricDecryptor) {
            parentConstructorError = true;
        }
    } else if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_StreamCipherAbstract)) {
        // retrieve native stream cipher objects
        symmetricEncryptor = getCryptoppStreamCipherEncryptorPtr(cipherObject);
        symmetricDecryptor = getCryptoppStreamCipherDecryptorPtr(cipherObject);

        if (NULL == symmetricEncryptor || NULL == symmetricDecryptor) {
            parentConstructorError = true;
        }
    } else {
        // create a proxy to the user php object
        try {
            symmetricEncryptor      = new SymmetricTransformationProxy::Encryption(cipherObject);
            symmetricDecryptor      = new SymmetricTransformationProxy::Decryption(cipherObject);
            cipherMustBeDestructed  = true;
        } catch (bool e) {
            return;
        } catch (const char *e) {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp internal error: SymmetricTransformationFilter: %s", e);
            return;
        }
    }

    if (parentConstructorError) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(cipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : parent constructor was not called", ce->name);
        return;
    }

    // create native symmetric transformation filter
    SymmetricTransformationFilter *stfEncryptor;
    SymmetricTransformationFilter *stfDecryptor;

    try {
        stfEncryptor = new SymmetricTransformationFilter(*symmetricEncryptor, paddingObject, cipherMustBeDestructed);
        stfDecryptor = new SymmetricTransformationFilter(*symmetricDecryptor, paddingObject, cipherMustBeDestructed);
    } catch (std::exception &e) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp internal error: SymmetricTransformationFilter: %s", e.what());
        return;
    }

    setCryptoppSymmetricTransformationFilterEncryptorPtr(getThis(), stfEncryptor TSRMLS_CC);
    setCryptoppSymmetricTransformationFilterDecryptorPtr(getThis(), stfDecryptor TSRMLS_CC);

    // hold the cipher mode object. if not, it can be deleted and associated encryptor/decryptor objects will be deleted too
    // same for padding
    zend_update_property(cryptopp_ce_SymmetricTransformationFilter, getThis(), "cipher", 6, cipherObject TSRMLS_CC);
    zend_update_property(cryptopp_ce_SymmetricTransformationFilter, getThis(), "padding", 7, paddingObject TSRMLS_CC);

    if (createdPadding) {
        Z_DELREF_P(paddingObject);
    }
}
/* }}} */

/* {{{ proto Cryptopp\SymmetricTransformationInterface SymmetricTransformationFilter::getCipher(void)
       Returns the cipher object */
PHP_METHOD(Cryptopp_SymmetricTransformationFilter, getCipher) {
    zval *cipherObject;
    cipherObject = zend_read_property(cryptopp_ce_SymmetricTransformationFilter, getThis(), "cipher", 6, 0 TSRMLS_CC);
    RETURN_ZVAL(cipherObject, 1, 0)
}
/* }}} */

/* {{{ proto Cryptopp\PaddingInterface SymmetricTransformationFilter::getPadding(void)
       Returns the padding object */
PHP_METHOD(Cryptopp_SymmetricTransformationFilter, getPadding) {
    zval *paddingObject;
    paddingObject = zend_read_property(cryptopp_ce_SymmetricTransformationFilter, getThis(), "padding", 7, 0 TSRMLS_CC);
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
    stfEncryptor = CRYPTOPP_SYMMETRIC_TRANSFORMATION_FILTER_GET_ENCRYPTOR_PTR(stfEncryptor)

    // if the mode object is a native object, ensure that the key/iv is valid
    if (!isNativeSymmetricTransformationObjectValid(getThis())) {
        RETURN_FALSE
    }

    // encrypt
    try {
        stfEncryptor->GetNextMessage();
        stfEncryptor->Put(reinterpret_cast<byte*>(data), dataSize);
        stfEncryptor->MessageEnd();

        CryptoPP::lword retrievable = stfEncryptor->MaxRetrievable();
        restartSymmetricCipherObject(getThis());

        if (retrievable >= dataSize) {
            // return ciphertext
            byte ciphertext[retrievable];
            stfEncryptor->Get(ciphertext, retrievable);
            RETURN_STRINGL(reinterpret_cast<char*>(ciphertext), retrievable, 1)
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
    stfDecryptor = CRYPTOPP_SYMMETRIC_TRANSFORMATION_FILTER_GET_DECRYPTOR_PTR(stfDecryptor)

    // if the mode object is a native object, ensure that the key/iv is valid
    if (!isNativeSymmetricTransformationObjectValid(getThis())) {
        RETURN_FALSE
    }

    // decrypt
    try {
        stfDecryptor->GetNextMessage();
        size_t written = stfDecryptor->Put(reinterpret_cast<byte*>(ciphertext), ciphertextSize);
        stfDecryptor->MessageEnd();

        CryptoPP::lword retrievable = stfDecryptor->MaxRetrievable();
        restartSymmetricCipherObject(getThis());

        if (retrievable > 0 && retrievable <= ciphertextSize) {
            // return plain text
            byte plaintext[retrievable];
            stfDecryptor->Get(plaintext, retrievable);
            RETURN_STRINGL(reinterpret_cast<char*>(plaintext), retrievable, 1)
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
