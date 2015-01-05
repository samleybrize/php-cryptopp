#include "../../php_cryptopp.h"
#include "../../exception/php_exception.h"
#include "../cipher/php_symmetric_cipher_interface.h"
#include "../cipher/php_symmetric_transformation_interface.h"
#include "../cipher/block/block_cipher_proxy.h"
#include "../cipher/block/php_block_cipher_interface.h"
#include "../cipher/block/php_block_cipher_abstract.h"
#include "php_symmetric_mode.h"
#include "php_symmetric_mode_interface.h"
#include "php_symmetric_mode_abstract.h"
#include <zend_exceptions.h>
#include <string>

/* {{{ custom object create/free handler */
zend_object_handlers SymmetricModeAbstract_object_handlers;

void SymmetricModeAbstract_free_storage(void *object TSRMLS_DC) {
    SymmetricModeAbstractContainer *obj = static_cast<SymmetricModeAbstractContainer *>(object);
    delete obj->encryptor;
    delete obj->decryptor;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}

zend_object_value SymmetricModeAbstract_create_handler(zend_class_entry *type TSRMLS_DC) {
    zend_object_value retval;

    SymmetricModeAbstractContainer *obj = static_cast<SymmetricModeAbstractContainer *>(emalloc(sizeof(SymmetricModeAbstractContainer)));
    memset(obj, 0, sizeof(SymmetricModeAbstractContainer));

    zend_object_std_init(&obj->std, type TSRMLS_CC);

    #if PHP_VERSION_ID < 50399
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    #else
        object_properties_init(static_cast<zend_object*>(&(obj->std)), type);
    #endif

    retval.handle   = zend_objects_store_put(obj, NULL, SymmetricModeAbstract_free_storage, NULL TSRMLS_CC);
    retval.handlers = &SymmetricModeAbstract_object_handlers;

    return retval;
}
/* }}} */

/* {{{ PHP abstract class declaration */
zend_class_entry *cryptopp_ce_SymmetricModeAbstract;

static zend_function_entry cryptopp_methods_SymmetricModeAbstract[] = {
    PHP_ME(Cryptopp_SymmetricModeAbstract, __sleep, arginfo_SymmetricModeAbstract___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricModeAbstract, __wakeup, arginfo_SymmetricModeAbstract___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricModeAbstract, getName, arginfo_SymmetricCipherInterface_getName, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricModeAbstract, getBlockSize, arginfo_SymmetricCipherInterface_getBlockSize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricModeAbstract, isValidKeyLength, arginfo_SymmetricCipherInterface_isValidKeyLength, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricModeAbstract, isValidIvLength, arginfo_SymmetricTransformationInterface_isValidIvLength, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricModeAbstract, setKey, arginfo_SymmetricCipherInterface_setKey, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricModeAbstract, setIv, arginfo_SymmetricTransformationInterface_setIv, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricModeAbstract, encrypt, arginfo_SymmetricTransformationInterface_encrypt, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricModeAbstract, decrypt, arginfo_SymmetricTransformationInterface_decrypt, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricModeAbstract, restart, arginfo_SymmetricTransformationInterface_restart, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_SymmetricModeAbstract(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "SymmetricModeAbstract", cryptopp_methods_SymmetricModeAbstract);
    cryptopp_ce_SymmetricModeAbstract               = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_SymmetricModeAbstract->ce_flags    |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

    cryptopp_ce_SymmetricModeAbstract->create_object = SymmetricModeAbstract_create_handler;
    memcpy(&SymmetricModeAbstract_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    SymmetricModeAbstract_object_handlers.clone_obj = NULL;

    zend_class_implements(cryptopp_ce_SymmetricModeAbstract TSRMLS_CC, 1, cryptopp_ce_SymmetricModeInterface);

    zend_declare_property_null(cryptopp_ce_SymmetricModeAbstract, "cipher", 6, ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_string(cryptopp_ce_SymmetricModeAbstract, "name", 4, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_string(cryptopp_ce_SymmetricModeAbstract, "key", 3, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_string(cryptopp_ce_SymmetricModeAbstract, "iv", 2, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ inits a child class */
void init_class_SymmetricModeAbstractChild(const char *modeName, const char* className, zend_class_entry **classEntryPtr, zend_function_entry *classMethods TSRMLS_DC) {
    std::string namespacedClassName("Cryptopp\\");
    namespacedClassName.append(className);

    zend_class_entry ce;
    INIT_CLASS_ENTRY_EX(ce, namespacedClassName.c_str(), namespacedClassName.length(), classMethods);
    *classEntryPtr = zend_register_internal_class_ex(&ce, cryptopp_ce_SymmetricModeAbstract, NULL TSRMLS_CC);

    addSymmetricMode(modeName, namespacedClassName.c_str());
}
/* }}} */

/* {{{ get the pointer to the native encryptor object of a php mode class */
CryptoPP::SymmetricCipher *getCryptoppSymmetricModeEncryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::SymmetricCipher *encryptor;
    encryptor = static_cast<SymmetricModeAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->encryptor;

    if (NULL == encryptor) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\SymmetricModeAbstract cannot be extended by user classes");
    }

    return encryptor;
}
/* }}} */

/* {{{ get the pointer to the native decryptor object of a php mode class */
CryptoPP::SymmetricCipher *getCryptoppSymmetricModeDecryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::SymmetricCipher *decryptor;
    decryptor = static_cast<SymmetricModeAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->decryptor;

    if (NULL == decryptor) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\SymmetricModeAbstract cannot be extended by user classes");
    }

    return decryptor;
}
/* }}} */

/* {{{ set the pointer to the native encryptor object of a php mode class */
void setCryptoppSymmetricModeEncryptorPtr(zval *this_ptr, CryptoPP::SymmetricCipher *encryptorPtr TSRMLS_DC) {
    if (encryptorPtr->MandatoryBlockSize() < 1) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\SymmetricModeAbstract can only be used with a block cipher with a block size greater than 0");
        return;
    }

    static_cast<SymmetricModeAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->encryptor = encryptorPtr;
}
/* }}} */

/* {{{ set the pointer to the native decryptor object of a php mode class */
void setCryptoppSymmetricModeDecryptorPtr(zval *this_ptr, CryptoPP::SymmetricCipher *decryptorPtr TSRMLS_DC) {
    if (decryptorPtr->MandatoryBlockSize() < 1) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\SymmetricModeAbstract can only be used with a block cipher with a block size greater than 0");
        return;
    }

    static_cast<SymmetricModeAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->decryptor = decryptorPtr;
}
/* }}} */

/* {{{ Get needed cipher elements to build a mode object */
bool cryptoppSymmetricModeGetCipherElements(
    const char *modeName,
    zval *cipherObject,
    zval *modeObject,
    CryptoPP::BlockCipher **cipherEncryptor,
    CryptoPP::BlockCipher **cipherDecryptor,
    std::string **modeFullName
) {
    if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_BlockCipherAbstract)) {
        // retrieve native objects
        *cipherEncryptor = static_cast<BlockCipherAbstractContainer *>(zend_object_store_get_object(cipherObject TSRMLS_CC))->encryptor;
        *cipherDecryptor = static_cast<BlockCipherAbstractContainer *>(zend_object_store_get_object(cipherObject TSRMLS_CC))->decryptor;
    } else if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_BlockCipherInterface)) {
        // create a proxy to the user php object
        *cipherEncryptor = new BlockCipherProxy::Encryption(cipherObject);
        *cipherDecryptor = new BlockCipherProxy::Decryption(cipherObject);
    } else {
        // invalid object
        zend_class_entry *ce;
        ce  = zend_get_class_entry(modeObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Internal error: %s : invalid cipher object", ce->name);
        return false;
    }

    // verify that cipher encryptor/decryptor ptr are not null
    if (NULL == cipherEncryptor || NULL == cipherDecryptor) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(cipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : parent constructor was not called", ce->name);
        return false;
    }

    // retrieve the name of the cipher
    zval *zCipherName;
    zval *funcName;
    MAKE_STD_ZVAL(zCipherName);
    MAKE_STD_ZVAL(funcName);
    ZVAL_STRING(funcName, "getName", 1);
    call_user_function(NULL, &cipherObject, funcName, zCipherName, 0, NULL TSRMLS_CC);

    // build mode name with cipher name
    *modeFullName = new std::string(modeName);
    (*modeFullName)->append("(");
    (*modeFullName)->append(Z_STRVAL_P(zCipherName), Z_STRLEN_P(zCipherName));
    (*modeFullName)->append(")");

    zval_dtor(zCipherName);
    zval_dtor(funcName);

    return true;
}
/* }}} */

/* {{{ verify that a key size is valid for a SymmetricModeAbstract instance */
static bool isCryptoppSymmetricModeKeyValid(zval *object, CryptoPP::SymmetricCipher *mode, int keySize) {
    zend_class_entry *ce;
    ce = zend_get_class_entry(object TSRMLS_CC);

    if (!mode->IsValidKeyLength(keySize)) {
        if (0 == keySize) {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : a key is required", ce->name, keySize);
        } else {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : %d is not a valid key length", ce->name, keySize);
        }

        return false;
    }

    return true;
}

bool isCryptoppSymmetricModeKeyValid(zval *object, CryptoPP::SymmetricCipher *mode) {
    zval *key;
    key         = zend_read_property(cryptopp_ce_SymmetricModeAbstract, object, "key", 3, 1 TSRMLS_CC);
    int keySize = Z_STRLEN_P(key);

    return isCryptoppSymmetricModeKeyValid(object, mode, keySize);
}
/* }}} */

/* {{{ verify that an iv size is valid for a SymmetricModeAbstract instance */
static bool isCryptoppSymmetricModeIvValid(zval *object, CryptoPP::SymmetricCipher *mode, int ivSize) {
    zend_class_entry *ce;
    ce = zend_get_class_entry(object TSRMLS_CC);

    if (mode->IsResynchronizable() &&
            (ivSize < mode->MinIVLength() || ivSize > mode->MaxIVLength())) {
        if (0 == ivSize) {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : an initialization vector is required", ce->name, ivSize);
        } else {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : %d is not a valid initialization vector length", ce->name, ivSize);
        }

        return false;
    }

    return true;
}

bool isCryptoppSymmetricModeIvValid(zval *object, CryptoPP::SymmetricCipher *mode) {
    zval *iv;
    iv          = zend_read_property(cryptopp_ce_SymmetricModeAbstract, object, "iv", 2, 1 TSRMLS_CC);
    int ivSize  = Z_STRLEN_P(iv);

    return isCryptoppSymmetricModeIvValid(object, mode, ivSize);
}
/* }}} */

/* {{{ sets the key and the iv (if applicable) of the native mode objects of a mode php object */
static void setKeyWithIv(zval *object, CryptoPP::SymmetricCipher *encryptor, CryptoPP::SymmetricCipher *decryptor) {
    // get key and iv of the php object
    zval *zKey;
    zval *zIv;
    zKey        = zend_read_property(cryptopp_ce_SymmetricModeAbstract, object, "key", 3, 1 TSRMLS_CC);
    zIv         = zend_read_property(cryptopp_ce_SymmetricModeAbstract, object, "iv", 2, 1 TSRMLS_CC);
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

/* {{{ proto void SymmetricModeAbstract::__sleep(void)
   Prevents serialization of a SymmetricModeAbstract instance */
PHP_METHOD(Cryptopp_SymmetricModeAbstract, __sleep) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\SymmetricModeAbstract instances");
}
/* }}} */

/* {{{ proto void SymmetricModeAbstract::__wakeup(void)
   Prevents use of a SymmetricModeAbstract instance that has been unserialized */
PHP_METHOD(Cryptopp_SymmetricModeAbstract, __wakeup) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\SymmetricModeAbstract instances");
}
/* }}} */

/* {{{ proto string SymmetricModeAbstract::getName(void)
   Return algorithm name */
PHP_METHOD(Cryptopp_SymmetricModeAbstract, getName) {
    CryptoPP::SymmetricCipher *encryptor;
    encryptor = CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)

    zval *name;
    name = zend_read_property(cryptopp_ce_SymmetricModeAbstract, getThis(), "name", 4, 0 TSRMLS_CC);
    RETURN_ZVAL(name, 1, 0);
}
/* }}} */

/* {{{ proto int SymmetricModeAbstract::getBlockSize(void)
   Returns the block size */
PHP_METHOD(Cryptopp_SymmetricModeAbstract, getBlockSize) {
    CryptoPP::SymmetricCipher *encryptor;
    encryptor = CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    RETURN_LONG(encryptor->MandatoryBlockSize())
}
/* }}} */

/* {{{ proto bool SymmetricModeAbstract::isValidKeyLength(int keyLength)
   Indicates if a key length is valid */
PHP_METHOD(Cryptopp_SymmetricModeAbstract, isValidKeyLength) {
    int keySize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &keySize)) {
        return;
    }

    CryptoPP::SymmetricCipher *encryptor;
    encryptor = CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);

    if (encryptor->IsValidKeyLength(keySize)) {
        RETURN_TRUE
    } else {
        RETURN_FALSE
    }
}
/* }}} */

/* {{{ proto bool SymmetricModeAbstract::isValidIvLength(int length)
   Indicates if an iv length is valid */
PHP_METHOD(Cryptopp_SymmetricModeAbstract, isValidIvLength) {
    int ivSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &ivSize)) {
        return;
    }

    CryptoPP::SymmetricCipher *encryptor;
    encryptor = CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);

    if (ivSize < encryptor->MinIVLength() || ivSize > encryptor->MaxIVLength()) {
        RETURN_FALSE
    } else {
        RETURN_TRUE
    }
}
/* }}} */

/* {{{ proto void SymmetricModeAbstract::setKey(string key)
   Sets the key */
PHP_METHOD(Cryptopp_SymmetricModeAbstract, setKey) {
    char *key   = NULL;
    int keySize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &keySize)) {
        return;
    }

    CryptoPP::SymmetricCipher *encryptor;
    CryptoPP::SymmetricCipher *decryptor;
    encryptor = CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    decryptor = CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);

    if (!isCryptoppSymmetricModeKeyValid(getThis(), encryptor, keySize)) {
        RETURN_FALSE;
    }

    // set the key on both the php object and the native cryptopp object
    zend_update_property_stringl(cryptopp_ce_SymmetricModeAbstract, getThis(), "key", 3, key, keySize TSRMLS_CC);
    setKeyWithIv(getThis(), encryptor, decryptor);
}
/* }}} */

/* {{{ proto void SymmetricModeAbstract::setIv(string iv)
   Sets the initialization vector */
PHP_METHOD(Cryptopp_SymmetricModeAbstract, setIv) {
    char *iv    = NULL;
    int ivSize  = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &iv, &ivSize)) {
        return;
    }

    CryptoPP::SymmetricCipher *encryptor;
    CryptoPP::SymmetricCipher *decryptor;
    encryptor = CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    decryptor = CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);

    if (!encryptor->IsResynchronizable()) {
        // an initialization vector is not required
        return;
    } else if (!isCryptoppSymmetricModeIvValid(getThis(), encryptor, ivSize)) {
        // invalid iv
        RETURN_FALSE;
    }

    // set the iv on both the php object and the native cryptopp object
    zend_update_property_stringl(cryptopp_ce_SymmetricModeAbstract, getThis(), "iv", 2, iv, ivSize TSRMLS_CC);
    setKeyWithIv(getThis(), encryptor, decryptor);
}
/* }}} */

/* {{{ proto string SymmetricModeAbstract::encrypt(string data)
   Encrypts data */
PHP_METHOD(Cryptopp_SymmetricModeAbstract, encrypt) {
    char *data      = NULL;
    int dataSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &dataSize)) {
        return;
    }

    CryptoPP::SymmetricCipher *encryptor;
    encryptor = CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);

    // check key and iv
    if (!isCryptoppSymmetricModeKeyValid(getThis(), encryptor) || !isCryptoppSymmetricModeIvValid(getThis(), encryptor)) {
        RETURN_FALSE
    }

    // check dataSize against block size
    int blockSize = static_cast<int>(encryptor->MandatoryBlockSize());

    if (0 != dataSize % blockSize) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(getThis() TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s: data size (%d) is not a multiple of block size (%d)", ce->name, dataSize, blockSize);
        RETURN_FALSE
    }

    // encrypt
    byte output[dataSize];

    try {
        encryptor->ProcessData(output, reinterpret_cast<byte*>(data), dataSize);
    } catch (bool e) {
        RETURN_FALSE
    }

    RETURN_STRINGL(reinterpret_cast<char*>(output), dataSize, 1)
}
/* }}} */

/* {{{ proto string SymmetricModeAbstract::decrypt(string data)
   Decrypts data */
PHP_METHOD(Cryptopp_SymmetricModeAbstract, decrypt) {
    char *data      = NULL;
    int dataSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &dataSize)) {
        return;
    }

    CryptoPP::SymmetricCipher *decryptor;
    decryptor = CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);

    // check key and iv
    if (!isCryptoppSymmetricModeKeyValid(getThis(), decryptor) || !isCryptoppSymmetricModeIvValid(getThis(), decryptor)) {
        RETURN_FALSE
    }

    // check dataSize against block size
    int blockSize = static_cast<int>(decryptor->MandatoryBlockSize());

    if (0 != dataSize % blockSize) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(getThis() TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s: data size (%d) is not a multiple of block size (%d)", ce->name, dataSize, blockSize);
        RETURN_FALSE
    }

    // encrypt
    byte output[dataSize];

    try {
        decryptor->ProcessData(output, reinterpret_cast<byte*>(data), dataSize);
    } catch (bool e) {
        RETURN_FALSE
    }

    RETURN_STRINGL(reinterpret_cast<char*>(output), dataSize, 1)
}
/* }}} */

/* {{{ proto void SymmetricModeAbstract::restart(void)
   Reset the initialization vector to its initial state (the one passed in setIv()) */
PHP_METHOD(Cryptopp_SymmetricModeAbstract, restart) {
    CryptoPP::SymmetricCipher *encryptor;
    CryptoPP::SymmetricCipher *decryptor;
    encryptor = CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    decryptor = CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);
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
