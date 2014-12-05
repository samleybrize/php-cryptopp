#include "../php_cryptopp.h"
#include "php_stream_transformation_filter.h"
#include "../symmetric/mode/php_symmetric_mode_interface.h"
#include "../symmetric/mode/php_symmetric_mode_abstract.h"
#include "../exception/php_exception.h"
#include <filters.h>
#include <zend_exceptions.h>

/* {{{ arg info */
ZEND_BEGIN_ARG_INFO(arginfo_StreamTransformationFilter___construct, 0)
    ZEND_ARG_OBJ_INFO(0, cipherMode, Cryptopp\\SymmetricModeInterface, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_StreamTransformationFilter___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_StreamTransformationFilter___sleep, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_StreamTransformationFilter_getCipherMode, 0)
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
    PHP_ME(Cryptopp_StreamTransformationFilter, __construct, arginfo_StreamTransformationFilter___construct, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamTransformationFilter, __sleep, arginfo_StreamTransformationFilter___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamTransformationFilter, __wakeup, arginfo_StreamTransformationFilter___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamTransformationFilter, getCipherMode, arginfo_StreamTransformationFilter_getCipherMode, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
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

    zend_declare_property_null(cryptopp_ce_StreamTransformationFilter, "cipherMode", 10, ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ get the pointer to the native stf encryptor object of the php class */
static CryptoPP::StreamTransformationFilter *getCryptoppStreamTransformationFilterEncryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::StreamTransformationFilter *stf;
    stf = static_cast<StreamTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfEncryptor;

    if (NULL == stf) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\StreamTransformationFilter : constructor was not called");
    }

    return stf;
}
/* }}} */

/* {{{ get the pointer to the native stf decryptor object of the php class */
static CryptoPP::StreamTransformationFilter *getCryptoppStreamTransformationFilterDecryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::StreamTransformationFilter *stf;
    stf = static_cast<StreamTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfDecryptor;

    if (NULL == stf) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\StreamTransformationFilter : constructor was not called");
    }

    return stf;
}
/* }}} */

/* {{{ set the pointer to the native stf encryptor object of the php class */
static void setCryptoppStreamTransformationFilterEncryptorPtr(zval *this_ptr, CryptoPP::StreamTransformationFilter *nativePtr TSRMLS_DC) {
    static_cast<StreamTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfEncryptor = nativePtr;
}
/* }}} */

/* {{{ set the pointer to the native stf decryptor object of the php class */
static void setCryptoppStreamTransformationFilterDecryptorPtr(zval *this_ptr, CryptoPP::StreamTransformationFilter *nativePtr TSRMLS_DC) {
    static_cast<StreamTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfDecryptor = nativePtr;
}
/* }}} */

/* {{{ returns the mode encryptor */
static CryptoPP::CipherModeBase *getModeEncryptor(zval *modeObject) {
    return static_cast<SymmetricModeAbstractContainer *>(zend_object_store_get_object(modeObject TSRMLS_CC))->encryptor;
}
/* }}} */

/* {{{ returns the mode decryptor */
static CryptoPP::CipherModeBase *getModeDecryptor(zval *modeObject) {
    return static_cast<SymmetricModeAbstractContainer *>(zend_object_store_get_object(modeObject TSRMLS_CC))->decryptor;
}
/* }}} */

/* {{{ indicates if the native mode object holded by a stf object is valid */
static bool isNativeModeObjectValid(zval *stfObject) {
    zval *modeObject;
    modeObject = zend_read_property(cryptopp_ce_StreamTransformationFilter, stfObject, "cipherMode", 10, 0 TSRMLS_CC);

    if (instanceof_function(Z_OBJCE_P(modeObject), cryptopp_ce_SymmetricModeAbstract)) {
        CryptoPP::CipherModeBase *modeEncryptor;
        modeEncryptor = getModeEncryptor(modeObject);

        if (!isCryptoppSymmetricModeKeyValid(modeObject, modeEncryptor) || !isCryptoppSymmetricModeIvValid(modeObject, modeEncryptor)) {
            return false;
        }
    }

    return true;
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

/* {{{ proto StreamTransformationFilter::__construct(Cryptopp\SymmetricModeInterface cipherMode) */
PHP_METHOD(Cryptopp_StreamTransformationFilter, __construct) {
    // TODO padding?
    zval *modeObject;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &modeObject, cryptopp_ce_SymmetricModeInterface)) {
        return;
    }

    // create native stream transformation filter
    CryptoPP::StreamTransformationFilter *stfEncryptor;
    CryptoPP::StreamTransformationFilter *stfDecryptor;

    if (instanceof_function(Z_OBJCE_P(modeObject), cryptopp_ce_SymmetricModeAbstract)) {
        // retrieve native objects
        CryptoPP::CipherModeBase *modeEncryptor;
        CryptoPP::CipherModeBase *modeDecryptor;
        modeEncryptor = getModeEncryptor(modeObject);
        modeDecryptor = getModeDecryptor(modeObject);
        stfEncryptor = new CryptoPP::StreamTransformationFilter(*modeEncryptor);
        stfDecryptor = new CryptoPP::StreamTransformationFilter(*modeDecryptor);
    } else {
        // TODO use the proxy
    }

    setCryptoppStreamTransformationFilterEncryptorPtr(getThis(), stfEncryptor TSRMLS_CC);
    setCryptoppStreamTransformationFilterDecryptorPtr(getThis(), stfDecryptor TSRMLS_CC);

    // hold the cipher mode object. if not, it can be deleted and associated encryptor/decryptor objects will be deleted too
    zend_update_property(cryptopp_ce_StreamTransformationFilter, getThis(), "cipherMode", 10, modeObject TSRMLS_CC);
}
/* }}} */

/* {{{ proto Cryptopp\SymmetricModeInterface StreamTransformationFilter::getCipherMode()
       Returns the cipher mode */
PHP_METHOD(Cryptopp_StreamTransformationFilter, getCipherMode) {
    zval *cipherMode;
    cipherMode = zend_read_property(cryptopp_ce_StreamTransformationFilter, getThis(), "cipherMode", 10, 0 TSRMLS_CC);
    RETURN_ZVAL(cipherMode, 0, 0)
}
/* }}} */

// TODO method encrypt/decrypt: accept an input and output object (return boolean)
// TODO method encrypt/decrypt: accept a string (return string or false)

/* {{{ proto bool|string StreamTransformationFilter::encryptString(string data)
       Encrypts a string */
PHP_METHOD(Cryptopp_StreamTransformationFilter, encryptString) {
    char *data      = NULL;
    int dataSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &dataSize)) {
        return;
    }

    // if the mode object is a native object, ensure that the key/iv is valid
    if (!isNativeModeObjectValid(getThis())) {
        RETURN_FALSE
    }

    // encrypt
    CryptoPP::StreamTransformationFilter *stfEncryptor;
    stfEncryptor    = CRYPTOPP_STREAM_TRANSFORMATION_FILTER_GET_ENCRYPTOR_PTR(stfEncryptor)
    stfEncryptor->GetNextMessage();
    size_t written  = stfEncryptor->Put(reinterpret_cast<byte*>(data), dataSize);
    stfEncryptor->MessageEnd();

    CryptoPP::lword retrievable = stfEncryptor->MaxRetrievable();

    if (retrievable >= dataSize) {
        // return ciphertext
        byte ciphertext[retrievable];
        stfEncryptor->Get(ciphertext, retrievable);
        RETURN_STRINGL(reinterpret_cast<char*>(ciphertext), retrievable, 1)
    } else {
        // something goes wrong
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
    if (!isNativeModeObjectValid(getThis())) {
        RETURN_FALSE
    }

    // decrypt
    CryptoPP::StreamTransformationFilter *stfDecryptor;
    stfDecryptor    = CRYPTOPP_STREAM_TRANSFORMATION_FILTER_GET_DECRYPTOR_PTR(stfDecryptor)
    stfDecryptor->GetNextMessage();
    size_t written  = stfDecryptor->Put(reinterpret_cast<byte*>(ciphertext), ciphertextSize);
    stfDecryptor->MessageEnd();

    CryptoPP::lword retrievable = stfDecryptor->MaxRetrievable();

    if (retrievable > 0 && retrievable <= ciphertextSize) {
        // return plain text
        byte plaintext[retrievable];
        stfDecryptor->Get(plaintext, retrievable);
        RETURN_STRINGL(reinterpret_cast<char*>(plaintext), retrievable, 1)
    } else {
        // something goes wrong
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
