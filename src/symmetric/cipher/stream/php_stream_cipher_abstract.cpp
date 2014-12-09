#include "../../../php_cryptopp.h"
#include "../../../exception/php_exception.h"
#include "../php_symmetric_cipher_interface.h"
#include "../php_symmetric_transformation_interface.h"
#include "php_stream_cipher.h"
#include "php_stream_cipher_interface.h"
#include "php_stream_cipher_abstract.h"
#include <zend_exceptions.h>
#include <string>

/* {{{ custom object create/free handler */
zend_object_handlers StreamCipherAbstract_object_handlers;

void StreamCipherAbstract_free_storage(void *object TSRMLS_DC) {
    StreamCipherAbstractContainer *obj = static_cast<StreamCipherAbstractContainer *>(object);
    delete obj->encryptor;
    delete obj->decryptor;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}

zend_object_value StreamCipherAbstract_create_handler(zend_class_entry *type TSRMLS_DC) {
    zend_object_value retval;

    StreamCipherAbstractContainer *obj = static_cast<StreamCipherAbstractContainer *>(emalloc(sizeof(StreamCipherAbstractContainer)));
    memset(obj, 0, sizeof(StreamCipherAbstractContainer));

    zend_object_std_init(&obj->std, type TSRMLS_CC);

    #if PHP_VERSION_ID < 50399
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    #else
        object_properties_init(static_cast<zend_object*>(&(obj->std)), type);
    #endif

    retval.handle   = zend_objects_store_put(obj, NULL, StreamCipherAbstract_free_storage, NULL TSRMLS_CC);
    retval.handlers = &StreamCipherAbstract_object_handlers;

    return retval;
}
/* }}} */

/* {{{ PHP abstract class declaration */
zend_class_entry *cryptopp_ce_StreamCipherAbstract;

static zend_function_entry cryptopp_methods_StreamCipherAbstract[] = {
    PHP_ME(Cryptopp_StreamCipherAbstract, __sleep, arginfo_StreamCipherAbstract___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, __wakeup, arginfo_StreamCipherAbstract___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, getName, arginfo_SymmetricCipherInterface_getName, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, setKey, arginfo_SymmetricTransformationInterface_setKey, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, setIv, arginfo_SymmetricTransformationInterface_setIv, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, getBlockSize, arginfo_SymmetricTransformationInterface_getBlockSize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, encryptData, arginfo_SymmetricTransformationInterface_encryptData, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, decryptData, arginfo_SymmetricTransformationInterface_decryptData, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_StreamCipherAbstract, restart, arginfo_SymmetricTransformationInterface_restart, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_StreamCipherAbstract(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "StreamCipherAbstract", cryptopp_methods_StreamCipherAbstract);
    cryptopp_ce_StreamCipherAbstract            = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_StreamCipherAbstract->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

    cryptopp_ce_StreamCipherAbstract->create_object = StreamCipherAbstract_create_handler;
    memcpy(&StreamCipherAbstract_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    StreamCipherAbstract_object_handlers.clone_obj = NULL;

    zend_class_implements(cryptopp_ce_StreamCipherAbstract TSRMLS_CC, 1, cryptopp_ce_StreamCipherInterface);

    zend_declare_property_string(cryptopp_ce_StreamCipherAbstract, "name", 4, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_string(cryptopp_ce_StreamCipherAbstract, "key", 3, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_string(cryptopp_ce_StreamCipherAbstract, "iv", 2, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ inits a child class */
void init_class_StreamCipherAbstractChild(const char *algoName, const char* className, zend_class_entry *classEntryPtr, zend_function_entry *classMethods TSRMLS_DC) {
    std::string namespacedClassName("Cryptopp\\");
    namespacedClassName.append(className);

    zend_class_entry ce;
    INIT_CLASS_ENTRY_EX(ce, namespacedClassName.c_str(), namespacedClassName.length(), classMethods);
    classEntryPtr = zend_register_internal_class_ex(&ce, cryptopp_ce_StreamCipherAbstract, NULL TSRMLS_CC);

    addStreamCipherAlgo(algoName, namespacedClassName.c_str());
}
/* }}} */

/* {{{ get the pointer to the native encryptor object of a php cipher class */
CryptoPP::SymmetricCipher *getCryptoppStreamCipherEncryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::SymmetricCipher *encryptor;
    encryptor = static_cast<StreamCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->encryptor;

    if (NULL == encryptor) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\StreamCipherAbstract cannot be extended by user classes");
    }

    return encryptor;
}
/* }}} */

/* {{{ get the pointer to the native decryptor object of a php cipher class */
CryptoPP::SymmetricCipher *getCryptoppStreamCipherDecryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::SymmetricCipher *decryptor;
    decryptor = static_cast<StreamCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->decryptor;

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
static bool isCryptoppStreamCipherKeyValid(zval *object, CryptoPP::SymmetricCipher *cipher, int keySize) {
    zend_class_entry *ce;
    ce = zend_get_class_entry(object TSRMLS_CC);

    if (!cipher->IsValidKeyLength(keySize)) {
        if (0 == keySize) {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : a key is required", ce->name, keySize);
        } else {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : %d is not a valid key length", ce->name, keySize); // TODO indicates required key length
        }

        return false;
    }

    return true;
}

bool isCryptoppStreamCipherKeyValid(zval *object, CryptoPP::SymmetricCipher *cipher) {
    zval *key;
    key         = zend_read_property(cryptopp_ce_StreamCipherAbstract, object, "key", 3, 1 TSRMLS_CC);
    int keySize = Z_STRLEN_P(key);

    return isCryptoppStreamCipherKeyValid(object, cipher, keySize);
}
/* }}} */

/* {{{ verify that an iv size is valid for a StreamCipherAbstract instance */
static bool isCryptoppStreamCipherIvValid(zval *object, CryptoPP::SymmetricCipher *cipher, int ivSize) {
    zend_class_entry *ce;
    ce = zend_get_class_entry(object TSRMLS_CC);

    if (cipher->IsResynchronizable() && cipher->IVSize() != ivSize) {
        if (0 == ivSize) {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : an initialization vector is required", ce->name, ivSize);
        } else {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : %d is not a valid initialization vector length", ce->name, ivSize); // TODO indicates required iv length
        }

        return false;
    }

    return true;
}

bool isCryptoppStreamCipherIvValid(zval *object, CryptoPP::SymmetricCipher *cipher) {
    zval *iv;
    iv          = zend_read_property(cryptopp_ce_StreamCipherAbstract, object, "iv", 2, 1 TSRMLS_CC);
    int ivSize  = Z_STRLEN_P(iv);

    return isCryptoppStreamCipherIvValid(object, cipher, ivSize);
}
/* }}} */

/* {{{ sets the key and the iv (if applicable) of the native mode objects of a mode php object */
static void setKeyWithIv(zval *object, CryptoPP::SymmetricCipher *encryptor, CryptoPP::SymmetricCipher *decryptor) {
    // get key and iv of the php object
    zval *zKey;
    zval *zIv;
    zKey        = zend_read_property(cryptopp_ce_StreamCipherAbstract, object, "key", 3, 1 TSRMLS_CC);
    zIv         = zend_read_property(cryptopp_ce_StreamCipherAbstract, object, "iv", 2, 1 TSRMLS_CC);
    int keySize = Z_STRLEN_P(zKey);
    int ivSize  = Z_STRLEN_P(zIv);

    // set the key and the iv (if applicable) of native mode objects
    if (keySize > 0 && !encryptor->IsResynchronizable()) {
        // an iv is not required
        // set key
        byte *key;
        key = reinterpret_cast<byte*>(Z_STRVAL_P(zKey));
        encryptor->SetKey(key, keySize);
        decryptor->SetKey(key, keySize);
    } else if (keySize > 0 && ivSize > 0 && encryptor->IsResynchronizable()) {
        // set key and iv
        byte *key;
        byte *iv;
        key = reinterpret_cast<byte*>(Z_STRVAL_P(zKey));
        iv  = reinterpret_cast<byte*>(Z_STRVAL_P(zIv));
        encryptor->SetKeyWithIV(key, keySize, iv, ivSize);
        decryptor->SetKeyWithIV(key, keySize, iv, ivSize);
    }
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
    CryptoPP::SymmetricCipher *encryptor;
    encryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)

    zval *name;
    name = zend_read_property(cryptopp_ce_StreamCipherAbstract, getThis(), "name", 4, 0 TSRMLS_CC);
    RETURN_ZVAL(name, 1, 0);
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

    CryptoPP::SymmetricCipher *encryptor;
    CryptoPP::SymmetricCipher *decryptor;
    encryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    decryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);

    if (!isCryptoppStreamCipherKeyValid(getThis(), encryptor, keySize)) {
        RETURN_FALSE;
    }

    // set the key on both the php object and the native cryptopp object
    zend_update_property_stringl(cryptopp_ce_StreamCipherAbstract, getThis(), "key", 3, key, keySize TSRMLS_CC);
    setKeyWithIv(getThis(), encryptor, decryptor);
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

    CryptoPP::SymmetricCipher *encryptor;
    CryptoPP::SymmetricCipher *decryptor;
    encryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    decryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);

    if (!encryptor->IsResynchronizable()) {
        // an initialization vector is not required
        return;
    } else if (!isCryptoppStreamCipherIvValid(getThis(), encryptor, ivSize)) {
        // invalid iv
        RETURN_FALSE;
    }

    // set the iv on both the php object and the native cryptopp object
    zend_update_property_stringl(cryptopp_ce_StreamCipherAbstract, getThis(), "iv", 2, iv, ivSize TSRMLS_CC);
    setKeyWithIv(getThis(), encryptor, decryptor);
}
/* }}} */

/* {{{ proto int StreamCipherAbstract::getBlockSize()
   Returns the block size */
PHP_METHOD(Cryptopp_StreamCipherAbstract, getBlockSize) {
    // TODO
}
/* }}} */

/* {{{ proto string StreamCipherAbstract::encryptData(string data)
   Encrypts data */
PHP_METHOD(Cryptopp_StreamCipherAbstract, encryptData) {
    // TODO
}
/* }}} */

/* {{{ proto string StreamCipherAbstract::decryptData(string data)
   Decrypts data */
PHP_METHOD(Cryptopp_StreamCipherAbstract, decryptData) {
    // TODO
}
/* }}} */

/* {{{ proto void StreamCipherAbstract::restart()
   Reset the initialization vector to its initial state (the one passed in setIv()) */
PHP_METHOD(Cryptopp_StreamCipherAbstract, restart) {
    CryptoPP::SymmetricCipher *encryptor;
    CryptoPP::SymmetricCipher *decryptor;
    encryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    decryptor = CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);
    setKeyWithIv(getThis(), encryptor, decryptor);
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
