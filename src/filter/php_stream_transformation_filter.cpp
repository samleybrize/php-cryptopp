#include "../php_cryptopp.h"
#include "../exception/php_exception.h"
#include "../padding/php_padding_interface.h"
#include "../padding/php_pkcs7.h"
#include "../symmetric/cipher/stream/php_stream_cipher.h"
#include "../symmetric/cipher/stream/stream_cipher_proxy.h"
#include "../symmetric/mode/php_symmetric_mode_interface.h"
#include "../symmetric/mode/php_symmetric_mode_abstract.h"
#include "php_stream_transformation_filter.h"
#include <exception>
#include <filters.h>
#include <misc.h>
#include <zend_exceptions.h>

/* {{{ fork of CryptoPP::StreamTransformationFilter to support padding schemes as objects */
StreamTransformationFilter::StreamTransformationFilter(CryptoPP::StreamTransformation &c, zval *paddingObject)
    : CryptoPP::FilterWithBufferedInput(NULL)
    , m_cipher(c)
    , m_optimalBufferSize(0)
{
    // check paddingObject
    if (NULL == paddingObject || !instanceof_function(Z_OBJCE_P(paddingObject), cryptopp_ce_PaddingInterface)) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Internal error: expected instance of PaddingInterface");
        throw false;
    }

    m_paddingObject = paddingObject;

    // initialization copied from CryptoPP::StreamTransformationFilter
    assert(c.MinLastBlockSize() == 0 || c.MinLastBlockSize() > c.MandatoryBlockSize());

    if (dynamic_cast<CryptoPP::AuthenticatedSymmetricCipher *>(&c) != 0) {
        // TODO
        throw CryptoPP::InvalidArgument("StreamTransformationFilter: please use AuthenticatedEncryptionFilter and AuthenticatedDecryptionFilter for AuthenticatedSymmetricCipher");
    }

    IsolatedInitialize(MakeParameters(CryptoPP::Name::BlockPaddingScheme(), NO_PADDING, false));
}

bool StreamTransformationFilter::PaddingObjectCanUnpad()
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

void StreamTransformationFilter::InitializeDerivedAndReturnNewSizes(const CryptoPP::NameValuePairs &parameters, size_t &firstSize, size_t &blockSize, size_t &lastSize)
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

void StreamTransformationFilter::LastPut(const byte *inString, size_t length)
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

            if (IS_STRING != Z_TYPE_P(zOutput)) {
                throw false;
            }

            // ensure zOutput length is a multiple of block size
            int outputLength = Z_STRLEN_P(zOutput);

            if (0 != outputLength % blockSize) {
                zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"StreamTransformationFilter: cipher text last block length (%u) is not a multiple of block size (%u)", length, blockSize);
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
                zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"StreamTransformationFilter: cipher text last block length (%u) is not a multiple of block size (%u)", length, blockSize);
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
        zval_dtor(zInput);
        zval_dtor(zOutput);
        zval_dtor(zBlockSize);
        throw e;
    }

    // free zvals
    zval_dtor(funcName);
    zval_dtor(zInput);
    zval_dtor(zOutput);
    zval_dtor(zBlockSize);
}
/* }}} */

/* {{{ copied from CryptoPP::StreamTransformationFilter */
void StreamTransformationFilter::FirstPut(const byte *inString)
{
    m_optimalBufferSize = m_cipher.OptimalBlockSize();
    m_optimalBufferSize = (unsigned int)CryptoPP::STDMAX(m_optimalBufferSize, CryptoPP::RoundDownToMultipleOf(4096U, m_optimalBufferSize));
}

void StreamTransformationFilter::NextPutMultiple(const byte *inString, size_t length)
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

void StreamTransformationFilter::NextPutModifiable(byte *inString, size_t length)
{
    m_cipher.ProcessString(inString, length);
    AttachedTransformation()->PutModifiable(inString, length);
}
/* }}} */

/* {{{ arg info */
ZEND_BEGIN_ARG_INFO(arginfo_StreamTransformationFilter___construct, 0)
    ZEND_ARG_OBJ_INFO(0, cipherMode, Cryptopp\\StreamCipherInterface, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_StreamTransformationFilter___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_StreamTransformationFilter___sleep, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_StreamTransformationFilter_getStreamCipher, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_StreamTransformationFilter_encryptString, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_StreamTransformationFilter_decryptString, 0)
    ZEND_ARG_INFO(0, ciphertext)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ custom object create/free handler */
zend_object_handlers StreamTransformationFilter_object_handlers;

void StreamTransformationFilter_free_storage(void *object TSRMLS_DC) {
    StreamTransformationFilterContainer *obj = static_cast<StreamTransformationFilterContainer *>(object);
    delete obj->stfEncryptor;
    delete obj->stfDecryptor;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}

zend_object_value StreamTransformationFilter_create_handler(zend_class_entry *type TSRMLS_DC) {
    zend_object_value retval;

    StreamTransformationFilterContainer *obj = static_cast<StreamTransformationFilterContainer *>(emalloc(sizeof(StreamTransformationFilterContainer)));
    memset(obj, 0, sizeof(StreamTransformationFilterContainer));

    zend_object_std_init(&obj->std, type TSRMLS_CC);

    #if PHP_VERSION_ID < 50399
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    #else
        object_properties_init(static_cast<zend_object*>(&(obj->std)), type);
    #endif

    retval.handle   = zend_objects_store_put(obj, NULL, StreamTransformationFilter_free_storage, NULL TSRMLS_CC);
    retval.handlers = &StreamTransformationFilter_object_handlers;

    return retval;
}
/* }}} */

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_StreamTransformationFilter;

static zend_function_entry cryptopp_methods_StreamTransformationFilter[] = {
    PHP_ME(Cryptopp_StreamTransformationFilter, __construct, arginfo_StreamTransformationFilter___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Cryptopp_StreamTransformationFilter, __sleep, arginfo_StreamTransformationFilter___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamTransformationFilter, __wakeup, arginfo_StreamTransformationFilter___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamTransformationFilter, getStreamCipher, arginfo_StreamTransformationFilter_getStreamCipher, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamTransformationFilter, encryptString, arginfo_StreamTransformationFilter_encryptString, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamTransformationFilter, decryptString, arginfo_StreamTransformationFilter_decryptString, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_StreamTransformationFilter(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "StreamTransformationFilter", cryptopp_methods_StreamTransformationFilter);
    cryptopp_ce_StreamTransformationFilter = zend_register_internal_class(&ce TSRMLS_CC);

    cryptopp_ce_StreamTransformationFilter->create_object = StreamTransformationFilter_create_handler;
    memcpy(&StreamTransformationFilter_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    StreamTransformationFilter_object_handlers.clone_obj = NULL;

    zend_declare_property_null(cryptopp_ce_StreamTransformationFilter, "streamCipher", 12, ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_null(cryptopp_ce_StreamTransformationFilter, "padding", 7, ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ get the pointer to the native stf encryptor object of the php class */
static StreamTransformationFilter *getCryptoppStreamTransformationFilterEncryptorPtr(zval *this_ptr TSRMLS_DC) {
    StreamTransformationFilter *stf;
    stf = static_cast<StreamTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfEncryptor;

    if (NULL == stf) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\StreamTransformationFilter : constructor was not called");
    }

    return stf;
}
/* }}} */

/* {{{ get the pointer to the native stf decryptor object of the php class */
static StreamTransformationFilter *getCryptoppStreamTransformationFilterDecryptorPtr(zval *this_ptr TSRMLS_DC) {
    StreamTransformationFilter *stf;
    stf = static_cast<StreamTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfDecryptor;

    if (NULL == stf) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\StreamTransformationFilter : constructor was not called");
    }

    return stf;
}
/* }}} */

/* {{{ set the pointer to the native stf encryptor object of the php class */
static void setCryptoppStreamTransformationFilterEncryptorPtr(zval *this_ptr, StreamTransformationFilter *nativePtr TSRMLS_DC) {
    static_cast<StreamTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfEncryptor = nativePtr;
}
/* }}} */

/* {{{ set the pointer to the native stf decryptor object of the php class */
static void setCryptoppStreamTransformationFilterDecryptorPtr(zval *this_ptr, StreamTransformationFilter *nativePtr TSRMLS_DC) {
    static_cast<StreamTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfDecryptor = nativePtr;
}
/* }}} */

/* {{{ indicates if the native stream cipher object holded by a stf object is valid */
static bool isNativeStreamCipherObjectValid(zval *stfObject) {
    zval *streamCipherObject;
    streamCipherObject = zend_read_property(cryptopp_ce_StreamTransformationFilter, stfObject, "streamCipher", 12, 0 TSRMLS_CC);

    if (instanceof_function(Z_OBJCE_P(streamCipherObject), cryptopp_ce_SymmetricModeAbstract)) {
        // SymmetricModeAbstract
        CryptoPP::CipherModeBase *modeEncryptor;
        modeEncryptor = getCryptoppSymmetricModeEncryptorPtr(streamCipherObject);

        if (!isCryptoppSymmetricModeKeyValid(streamCipherObject, modeEncryptor) || !isCryptoppSymmetricModeIvValid(streamCipherObject, modeEncryptor)) {
            return false;
        }
    } else if (instanceof_function(Z_OBJCE_P(streamCipherObject), cryptopp_ce_StreamCipherAbstract)) {
        // StreamCipherAbstract
        CryptoPP::SymmetricCipher *streamCipherEncryptor;
        streamCipherEncryptor = getCryptoppSymmetricModeEncryptorPtr(streamCipherObject);

        if (!isCryptoppStreamCipherKeyValid(streamCipherObject, streamCipherEncryptor) || !isCryptoppStreamCipherIvValid(streamCipherObject, streamCipherEncryptor)) {
            return false;
        }
    } else {
        // TODO proxy
    }

    return true;
}
/* }}} */

/* {{{ restart the stream cipher holded by a StreamTransformationFilter php object */
static void restartStreamCipherObject(zval *stfObject) {
    zval *streamCipherObject;
    streamCipherObject = zend_read_property(cryptopp_ce_StreamTransformationFilter, stfObject, "streamCipher", 12, 0 TSRMLS_CC);

    zval *funcName;
    MAKE_STD_ZVAL(funcName)
    ZVAL_STRING(funcName, "restart", 1);
    call_user_function(NULL, &streamCipherObject, funcName, funcName, 0, NULL TSRMLS_CC);
    zval_dtor(funcName);
}
/* }}} */

/* {{{ proto void StreamTransformationFilter::__sleep(void)
   Prevents serialization of a StreamTransformationFilter instance */
PHP_METHOD(Cryptopp_StreamTransformationFilter, __sleep) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\StreamTransformationFilter instances");
}
/* }}} */

/* {{{ proto void StreamTransformationFilter::__wakeup(void)
   Prevents use of a StreamTransformationFilter instance that has been unserialized */
PHP_METHOD(Cryptopp_StreamTransformationFilter, __wakeup) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\StreamTransformationFilter instances");
}
/* }}} */

/* {{{ proto StreamTransformationFilter::__construct(Cryptopp\StreamCipherInterface streamCipher) */
PHP_METHOD(Cryptopp_StreamTransformationFilter, __construct) {
    zval *streamCipherObject;
    zval *paddingObject = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O|O", &streamCipherObject, cryptopp_ce_StreamCipherInterface, &paddingObject, cryptopp_ce_PaddingInterface)) {
        return;
    }

    // if no padding object, pick the default one
    bool createdPadding = false;

    if (NULL == paddingObject) {
        MAKE_STD_ZVAL(paddingObject);
        object_init_ex(paddingObject, cryptopp_ce_PaddingPkcs7);
        createdPadding = true;
    }

    // create native stream transformation filter
    StreamTransformationFilter *stfEncryptor;
    StreamTransformationFilter *stfDecryptor;

    try {
        if (instanceof_function(Z_OBJCE_P(streamCipherObject), cryptopp_ce_SymmetricModeAbstract)) {
            // retrieve native mode objects
            CryptoPP::CipherModeBase *modeEncryptor;
            CryptoPP::CipherModeBase *modeDecryptor;
            modeEncryptor = getCryptoppSymmetricModeEncryptorPtr(streamCipherObject);
            modeDecryptor = getCryptoppSymmetricModeDecryptorPtr(streamCipherObject);

            if (NULL == modeEncryptor || NULL == modeDecryptor) {
                zend_class_entry *ce;
                ce  = zend_get_class_entry(streamCipherObject TSRMLS_CC);
                zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : parent constructor was not called", ce->name);
                return;
            }

            stfEncryptor = new StreamTransformationFilter(*modeEncryptor, paddingObject);
            stfDecryptor = new StreamTransformationFilter(*modeDecryptor, paddingObject);
        } else if (instanceof_function(Z_OBJCE_P(streamCipherObject), cryptopp_ce_StreamCipherAbstract)) {
            // retrieve native stream cipher objects
            CryptoPP::SymmetricCipher *streamCipherEncryptor;
            CryptoPP::SymmetricCipher *streamCipherDecryptor;
            streamCipherEncryptor = getCryptoppStreamCipherEncryptorPtr(streamCipherObject);
            streamCipherDecryptor = getCryptoppStreamCipherDecryptorPtr(streamCipherObject);

            if (NULL == streamCipherEncryptor || NULL == streamCipherDecryptor) {
                zend_class_entry *ce;
                ce  = zend_get_class_entry(streamCipherObject TSRMLS_CC);
                zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : parent constructor was not called", ce->name);
                return;
            }

            stfEncryptor = new StreamTransformationFilter(*streamCipherEncryptor, paddingObject);
            stfDecryptor = new StreamTransformationFilter(*streamCipherDecryptor, paddingObject);
        } else {
            // TODO use the proxy
        }
    } catch (bool e) {
        return;
    }

    setCryptoppStreamTransformationFilterEncryptorPtr(getThis(), stfEncryptor TSRMLS_CC);
    setCryptoppStreamTransformationFilterDecryptorPtr(getThis(), stfDecryptor TSRMLS_CC);

    // hold the cipher mode object. if not, it can be deleted and associated encryptor/decryptor objects will be deleted too
    // same for padding
    zend_update_property(cryptopp_ce_StreamTransformationFilter, getThis(), "streamCipher", 12, streamCipherObject TSRMLS_CC);
    zend_update_property(cryptopp_ce_StreamTransformationFilter, getThis(), "padding", 7, paddingObject TSRMLS_CC);

    if (createdPadding) {
        Z_DELREF_P(paddingObject);
    }
}
/* }}} */

/* {{{ proto Cryptopp\SymmetricModeInterface StreamTransformationFilter::getStreamCipher()
       Returns the stream cipher object */
PHP_METHOD(Cryptopp_StreamTransformationFilter, getStreamCipher) {
    zval *cipherObject;
    cipherObject = zend_read_property(cryptopp_ce_StreamTransformationFilter, getThis(), "streamCipher", 12, 0 TSRMLS_CC);
    RETURN_ZVAL(cipherObject, 0, 0)
}
/* }}} */

// TODO method encrypt/decrypt: accept an input and output object (return boolean)

/* {{{ proto bool|string StreamTransformationFilter::encryptString(string data)
       Encrypts a string */
PHP_METHOD(Cryptopp_StreamTransformationFilter, encryptString) {
    char *data      = NULL;
    int dataSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &dataSize)) {
        return;
    }

    // if the mode object is a native object, ensure that the key/iv is valid
    if (!isNativeStreamCipherObjectValid(getThis())) {
        RETURN_FALSE
    }

    // encrypt
    try {
        StreamTransformationFilter *stfEncryptor;
        stfEncryptor    = CRYPTOPP_STREAM_TRANSFORMATION_FILTER_GET_ENCRYPTOR_PTR(stfEncryptor)
        stfEncryptor->GetNextMessage();
        size_t written  = stfEncryptor->Put(reinterpret_cast<byte*>(data), dataSize);
        stfEncryptor->MessageEnd();

        CryptoPP::lword retrievable = stfEncryptor->MaxRetrievable();
        restartStreamCipherObject(getThis());

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

/* {{{ proto bool|string StreamTransformationFilter::decryptString(string ciphertext)
       Decrypts a string */
PHP_METHOD(Cryptopp_StreamTransformationFilter, decryptString) {
    char *ciphertext    = NULL;
    int ciphertextSize  = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ciphertext, &ciphertextSize)) {
        return;
    }

    // if the mode object is a native object, ensure that the key/iv is valid
    if (!isNativeStreamCipherObjectValid(getThis())) {
        RETURN_FALSE
    }

    // decrypt
    try {
        StreamTransformationFilter *stfDecryptor;
        stfDecryptor    = CRYPTOPP_STREAM_TRANSFORMATION_FILTER_GET_DECRYPTOR_PTR(stfDecryptor)
        stfDecryptor->GetNextMessage();
        size_t written  = stfDecryptor->Put(reinterpret_cast<byte*>(ciphertext), ciphertextSize);
        stfDecryptor->MessageEnd();

        CryptoPP::lword retrievable = stfDecryptor->MaxRetrievable();
        restartStreamCipherObject(getThis());

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
