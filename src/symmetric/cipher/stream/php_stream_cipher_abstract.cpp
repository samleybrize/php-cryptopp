#include "src/php_cryptopp.h"
#include "src/exception/php_exception.h"
#include "src/utils/zend_object_utils.h"
#include "src/symmetric/cipher/php_symmetric_cipher_interface.h"
#include "src/symmetric/cipher/php_symmetric_transformation_interface.h"
#include "php_stream_cipher.h"
#include "php_stream_cipher_interface.h"
#include "php_stream_cipher_abstract.h"
#include <zend_exceptions.h>
#include <string>

/* {{{ custom object free handler */
void StreamCipherAbstract_free_storage(void *object TSRMLS_DC) {
    StreamCipherAbstractContainer *obj = static_cast<StreamCipherAbstractContainer *>(object);
    delete obj->encryptor;
    delete obj->decryptor;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}
/* }}} */

/* {{{ PHP abstract class declaration */
zend_object_handlers StreamCipherAbstract_object_handlers;
zend_class_entry *cryptopp_ce_StreamCipherAbstract;

static zend_function_entry cryptopp_methods_StreamCipherAbstract[] = {
    PHP_ME(Cryptopp_StreamCipherAbstract, __sleep, arginfo_StreamCipherAbstract___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, __wakeup, arginfo_StreamCipherAbstract___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, getName, arginfo_SymmetricCipherInterface_getName, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, getBlockSize, arginfo_SymmetricCipherInterface_getBlockSize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, isValidKeyLength, arginfo_SymmetricCipherInterface_isValidKeyLength, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, isValidIvLength, arginfo_SymmetricTransformationInterface_isValidIvLength, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, setKey, arginfo_SymmetricCipherInterface_setKey, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, setIv, arginfo_SymmetricTransformationInterface_setIv, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, getKey, arginfo_SymmetricCipherInterface_getKey, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, getIv, arginfo_SymmetricTransformationInterface_getIv, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, encrypt, arginfo_SymmetricTransformationInterface_encrypt, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, decrypt, arginfo_SymmetricTransformationInterface_decrypt, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, restart, arginfo_SymmetricTransformationInterface_restart, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_StreamCipherAbstract(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "StreamCipherAbstract", cryptopp_methods_StreamCipherAbstract);
    cryptopp_ce_StreamCipherAbstract                = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_StreamCipherAbstract->ce_flags     |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

    cryptopp_ce_StreamCipherAbstract->create_object = zend_custom_create_handler<StreamCipherAbstractContainer, StreamCipherAbstract_free_storage, &StreamCipherAbstract_object_handlers>;
    memcpy(&StreamCipherAbstract_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    StreamCipherAbstract_object_handlers.clone_obj  = NULL;

    zend_class_implements(cryptopp_ce_StreamCipherAbstract TSRMLS_CC, 1, cryptopp_ce_StreamCipherInterface);

    zend_declare_property_string(cryptopp_ce_StreamCipherAbstract, "name", 4, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_string(cryptopp_ce_StreamCipherAbstract, "key", 3, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_string(cryptopp_ce_StreamCipherAbstract, "iv", 2, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ inits a child class */
void init_class_StreamCipherAbstractChild(const char *algoName, const char* className, zend_class_entry **classEntryPtr, zend_function_entry *classMethods TSRMLS_DC) {
    std::string namespacedClassName("Cryptopp\\");
    namespacedClassName.append(className);

    zend_class_entry ce;
    INIT_CLASS_ENTRY_EX(ce, namespacedClassName.c_str(), namespacedClassName.length(), classMethods);
    *classEntryPtr = zend_register_internal_class_ex(&ce, cryptopp_ce_StreamCipherAbstract, NULL TSRMLS_CC);

    streamCipherAlgoList.addAlgo(algoName, namespacedClassName.c_str());
}
/* }}} */

/* {{{ get the pointer to the native encryptor object of a php cipher class */
CryptoPP::SymmetricCipher *getCryptoppStreamCipherEncryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::SymmetricCipher *encryptor = static_cast<StreamCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->encryptor;

    if (NULL == encryptor) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\StreamCipherAbstract cannot be extended by user classes");
    }

    return encryptor;
}
/* }}} */

/* {{{ get the pointer to the native decryptor object of a php cipher class */
CryptoPP::SymmetricCipher *getCryptoppStreamCipherDecryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::SymmetricCipher *decryptor = static_cast<StreamCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->decryptor;

    if (NULL == decryptor) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\StreamCipherAbstract cannot be extended by user classes");
    }

    return decryptor;
}
/* }}} */

/* {{{ set the pointer to the native encryptor object of a php cipher class */
void setCryptoppStreamCipherEncryptorPtr(zval *this_ptr, CryptoPP::SymmetricCipher *encryptorPtr TSRMLS_DC) {
    static_cast<StreamCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->encryptor = encryptorPtr;
}
/* }}} */

/* {{{ set the pointer to the native decryptor object of a php cipher class */
void setCryptoppStreamCipherDecryptorPtr(zval *this_ptr, CryptoPP::SymmetricCipher *decryptorPtr TSRMLS_DC) {
    static_cast<StreamCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->decryptor = decryptorPtr;
}
/* }}} */

/* {{{ verify that a key size is valid for a StreamCipherAbstract instance */
bool isCryptoppStreamCipherKeyValid(zval *object, CryptoPP::SymmetricCipher *cipher TSRMLS_DC) {
    zval *key   = zend_read_property(cryptopp_ce_StreamCipherAbstract, object, "key", 3, 1 TSRMLS_CC);
    int keySize = Z_STRLEN_P(key);

    return isCryptoppSymmetricKeyValid(object, cipher, keySize TSRMLS_CC);
}
/* }}} */

/* {{{ verify that an iv size is valid for a StreamCipherAbstract instance */
bool isCryptoppStreamCipherIvValid(zval *object, CryptoPP::SymmetricCipher *cipher TSRMLS_DC) {
    zval *iv    = zend_read_property(cryptopp_ce_StreamCipherAbstract, object, "iv", 2, 1 TSRMLS_CC);
    int ivSize  = Z_STRLEN_P(iv);

    return isCryptoppSymmetricIvValid(object, cipher, ivSize TSRMLS_CC);
}
/* }}} */

/* {{{ sets the key and the iv (if applicable) of the native cipher objects of a cipher php object */
static void setKeyWithIv(zval *object, CryptoPP::SymmetricCipher *encryptor, CryptoPP::SymmetricCipher *decryptor TSRMLS_DC) {
    zval *zKey      = zend_read_property(cryptopp_ce_StreamCipherAbstract, object, "key", 3, 1 TSRMLS_CC);
    zval *zIv       = zend_read_property(cryptopp_ce_StreamCipherAbstract, object, "iv", 2, 1 TSRMLS_CC);
    setSymmetricCipherKeyIv(object, encryptor, decryptor, zKey, zIv TSRMLS_CC);
}
/* }}} */

/* {{{ proto void StreamCipherAbstract::__sleep(void)
   Prevents serialization of a StreamCipherAbstract instance */
PHP_METHOD(Cryptopp_StreamCipherAbstract, __sleep) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\StreamCipherAbstract instances");
}
/* }}} */

/* {{{ proto void StreamCipherAbstract::__wakeup(void)
   Prevents use of a StreamCipherAbstract instance that has been unserialized */
PHP_METHOD(Cryptopp_StreamCipherAbstract, __wakeup) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\StreamCipherAbstract instances");
}
/* }}} */

/* {{{ proto string StreamCipherAbstract::getName(void)
   Return algorithm name */
PHP_METHOD(Cryptopp_StreamCipherAbstract, getName) {
    CryptoPP::SymmetricCipher *encryptor    = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)
    zval *name                              = zend_read_property(cryptopp_ce_StreamCipherAbstract, getThis(), "name", 4, 0 TSRMLS_CC);
    RETURN_ZVAL(name, 1, 0);
}
/* }}} */

/* {{{ proto int StreamCipherAbstract::getBlockSize(void)
   Returns the block size */
PHP_METHOD(Cryptopp_StreamCipherAbstract, getBlockSize) {
    CryptoPP::SymmetricCipher *encryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    RETURN_LONG(encryptor->MandatoryBlockSize())
}
/* }}} */

/* {{{ proto bool StreamCipherAbstract::isValidKeyLength(int keyLength)
   Indicates if a key length is valid */
PHP_METHOD(Cryptopp_StreamCipherAbstract, isValidKeyLength) {
    int keySize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &keySize)) {
        return;
    }

    CryptoPP::SymmetricCipher *encryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);

    if (isCryptoppSymmetricKeyValid(getThis(), encryptor, keySize TSRMLS_CC, false)) {
        RETURN_TRUE
    } else {
        RETURN_FALSE
    }
}
/* }}} */

/* {{{ proto bool StreamCipherAbstract::isValidIvLength(int length)
   Indicates if an iv length is valid */
PHP_METHOD(Cryptopp_StreamCipherAbstract, isValidIvLength) {
    int ivSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &ivSize)) {
        return;
    }

    CryptoPP::SymmetricCipher *encryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    encryptor->AlgorithmName(); // TODO without this statement, a segfault occur ?!

    if (isCryptoppSymmetricIvValid(getThis(), encryptor, ivSize TSRMLS_CC, false)) {
        RETURN_TRUE
    } else {
        RETURN_FALSE
    }
}
/* }}} */

/* {{{ proto void StreamCipherAbstract::setKey(string key)
   Sets the key */
PHP_METHOD(Cryptopp_StreamCipherAbstract, setKey) {
    char *key   = NULL;
    int keySize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &keySize)) {
        return;
    }

    CryptoPP::SymmetricCipher *encryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    CryptoPP::SymmetricCipher *decryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);

    if (!isCryptoppSymmetricKeyValid(getThis(), encryptor, keySize TSRMLS_CC)) {
        RETURN_FALSE;
    }

    // set the key on both the php object and the native cryptopp object
    zend_update_property_stringl(cryptopp_ce_StreamCipherAbstract, getThis(), "key", 3, key, keySize TSRMLS_CC);
    setKeyWithIv(getThis(), encryptor, decryptor TSRMLS_CC);
}
/* }}} */

/* {{{ proto void StreamCipherAbstract::setIv(string iv)
   Sets the initialization vector */
PHP_METHOD(Cryptopp_StreamCipherAbstract, setIv) {
    char *iv    = NULL;
    int ivSize  = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &iv, &ivSize)) {
        return;
    }

    CryptoPP::SymmetricCipher *encryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    CryptoPP::SymmetricCipher *decryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);

    if (!isCryptoppSymmetricIvValid(getThis(), encryptor, ivSize TSRMLS_CC)) {
        // invalid iv
        RETURN_FALSE;
    }

    // set the iv on both the php object and the native cryptopp object
    zend_update_property_stringl(cryptopp_ce_StreamCipherAbstract, getThis(), "iv", 2, iv, ivSize TSRMLS_CC);
    setKeyWithIv(getThis(), encryptor, decryptor TSRMLS_CC);
}
/* }}} */

/* {{{ proto string StreamCipherAbstract::getKey(void)
   Returns the key */
PHP_METHOD(Cryptopp_StreamCipherAbstract, getKey) {
    zval *key = zend_read_property(cryptopp_ce_StreamCipherAbstract, getThis(), "key", 3, 1 TSRMLS_CC);
    RETURN_ZVAL(key, 1, 0)
}
/* }}} */

/* {{{ proto string StreamCipherAbstract::getIv(void)
   Returns the initialization vector */
PHP_METHOD(Cryptopp_StreamCipherAbstract, getIv) {
    zval *iv = zend_read_property(cryptopp_ce_StreamCipherAbstract, getThis(), "iv", 2, 1 TSRMLS_CC);
    RETURN_ZVAL(iv, 1, 0)
}
/* }}} */

/* {{{ proto string StreamCipherAbstract::encrypt(string data)
   Encrypts data */
PHP_METHOD(Cryptopp_StreamCipherAbstract, encrypt) {
    char *data      = NULL;
    int dataSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &dataSize)) {
        return;
    }

    CryptoPP::SymmetricCipher *encryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);

    // check key and iv
    if (!isCryptoppStreamCipherKeyValid(getThis(), encryptor TSRMLS_CC) || !isCryptoppStreamCipherIvValid(getThis(), encryptor TSRMLS_CC)) {
        RETURN_FALSE
    }

    // check dataSize against block size
    int blockSize = static_cast<int>(encryptor->MandatoryBlockSize());

    if (0 != dataSize % blockSize) {
        zend_class_entry *ce = zend_get_class_entry(getThis() TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s: data size (%d) is not a multiple of block size (%d)", ce->name, dataSize, blockSize);
        RETURN_FALSE
    }

    // encrypt
    byte output[dataSize];
    encryptor->ProcessData(output, reinterpret_cast<byte*>(data), dataSize);

    RETURN_STRINGL(reinterpret_cast<char*>(output), dataSize, 1)
}
/* }}} */

/* {{{ proto string StreamCipherAbstract::decrypt(string data)
   Decrypts data */
PHP_METHOD(Cryptopp_StreamCipherAbstract, decrypt) {
    char *data      = NULL;
    int dataSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &dataSize)) {
        return;
    }

    CryptoPP::SymmetricCipher *decryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);

    // check key and iv
    if (!isCryptoppStreamCipherKeyValid(getThis(), decryptor TSRMLS_CC) || !isCryptoppStreamCipherIvValid(getThis(), decryptor TSRMLS_CC)) {
        RETURN_FALSE
    }

    // check dataSize against block size
    int blockSize = static_cast<int>(decryptor->MandatoryBlockSize());

    if (0 != dataSize % blockSize) {
        zend_class_entry *ce = zend_get_class_entry(getThis() TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s: data size (%d) is not a multiple of block size (%d)", ce->name, dataSize, blockSize);
        RETURN_FALSE
    }

    // encrypt
    byte output[dataSize];
    decryptor->ProcessData(output, reinterpret_cast<byte*>(data), dataSize);

    RETURN_STRINGL(reinterpret_cast<char*>(output), dataSize, 1)
}
/* }}} */

/* {{{ proto void StreamCipherAbstract::restart(void)
   Reset the initialization vector to its initial state (the one passed in setIv()) */
PHP_METHOD(Cryptopp_StreamCipherAbstract, restart) {
    CryptoPP::SymmetricCipher *encryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    CryptoPP::SymmetricCipher *decryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);
    setKeyWithIv(getThis(), encryptor, decryptor TSRMLS_CC);
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
