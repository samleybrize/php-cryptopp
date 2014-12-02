#include "../../php_cryptopp.h"
#include "php_symmetric_mode_abstract.h"
#include "php_symmetric_mode_interface.h"
#include "php_symmetric_mode.h"
#include "../../exception/php_exception.h"
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
    PHP_ME(Cryptopp_SymmetricModeAbstract, getName, arginfo_SymmetricModeInterface_getName, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricModeAbstract, setKey, arginfo_SymmetricModeInterface_setKey, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
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
}
/* }}} */

/* {{{ inits a child class */
void init_class_SymmetricModeAbstractChild(const char *modeName, const char* className, zend_class_entry *classEntryPtr, zend_function_entry *classMethods TSRMLS_DC) {
    std::string namespacedClassName("Cryptopp\\");
    namespacedClassName.append(className);

    zend_class_entry ce;
    INIT_CLASS_ENTRY_EX(ce, namespacedClassName.c_str(), namespacedClassName.length(), classMethods);
    classEntryPtr = zend_register_internal_class_ex(&ce, cryptopp_ce_SymmetricModeAbstract, NULL TSRMLS_CC);

    addSymmetricMode(modeName, namespacedClassName.c_str());
}
/* }}} */

/* {{{ get the pointer to the native encryptor object of a php mode class */
CryptoPP::CipherModeBase *getCryptoppSymmetricModeEncryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::CipherModeBase *encryptor;
    encryptor = static_cast<SymmetricModeAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->encryptor;

    if (NULL == encryptor) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\SymmetricModeAbstract cannot be extended by user classes");
    }

    return encryptor;
}
/* }}} */

/* {{{ get the pointer to the native decryptor object of a php mode class */
CryptoPP::CipherModeBase *getCryptoppSymmetricModeDecryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::CipherModeBase *decryptor;
    decryptor = static_cast<SymmetricModeAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->decryptor;

    if (NULL == decryptor) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\SymmetricModeAbstract cannot be extended by user classes");
    }

    return decryptor;
}
/* }}} */

/* {{{ set the pointer to the native encryptor object of a php mode class */
void setCryptoppSymmetricModeEncryptorPtr(zval *this_ptr, CryptoPP::CipherModeBase *encryptorPtr TSRMLS_DC) {
    static_cast<SymmetricModeAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->encryptor = encryptorPtr;
}
/* }}} */

/* {{{ set the pointer to the native decryptor object of a php mode class */
void setCryptoppSymmetricModeDecryptorPtr(zval *this_ptr, CryptoPP::CipherModeBase *decryptorPtr TSRMLS_DC) {
    static_cast<SymmetricModeAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->decryptor = decryptorPtr;
}
/* }}} */

/* {{{ Get needed cipher elements to build a mode object */
bool cryptoppSymmetricModeGetCipherElements(
    zval *cipherObject,
    zval *modeObject,
    CryptoPP::BlockCipher **cipherEncryptor,
    CryptoPP::BlockCipher **cipherDecryptor,
    std::string **cipherName
) {
    // get cipher encryptor/decryptor ptr
    *cipherEncryptor = static_cast<SymmetricCipherAbstractContainer *>(zend_object_store_get_object(cipherObject TSRMLS_CC))->encryptor;
    *cipherDecryptor = static_cast<SymmetricCipherAbstractContainer *>(zend_object_store_get_object(cipherObject TSRMLS_CC))->decryptor;

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

    *cipherName = new std::string(Z_STRVAL_P(zCipherName), Z_STRLEN_P(zCipherName));

    zval_dtor(zCipherName);
    zval_dtor(funcName);

    return true;
}
/* }}} */

/* {{{ verify that a key size is valid for a MacInterface instance */
static bool isKeyValid(zval *object, CryptoPP::CipherModeBase *mode) {
    zend_class_entry *ce;
    zval *key;
    ce          = zend_get_class_entry(object TSRMLS_CC);
    key         = zend_read_property(ce, object, "key", 3, 1 TSRMLS_CC);
    int keySize = Z_STRLEN_P(key);

    if (!mode->IsValidKeyLength(keySize)) {
        if (0 == keySize) {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : a key is required", ce->name, keySize);
        } else {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : %d is not a valid key length", ce->name, keySize); // TODO indicates required key length
        }

        return false;
    }

    return true;
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
    CryptoPP::CipherModeBase *encryptor;
    encryptor = CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)

    zval *name;
    name = zend_read_property(cryptopp_ce_SymmetricModeAbstract, getThis(), "name", 4, 0 TSRMLS_CC);
    RETURN_ZVAL(name, 1, 0);
}
/* }}} */

/* {{{ proto string SymmetricModeAbstract::setKey(string key)
   Sets the key */
PHP_METHOD(Cryptopp_SymmetricModeAbstract, setKey) {
    char *key   = NULL;
    int keySize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &keySize)) {
        return;
    }

    CryptoPP::CipherModeBase *encryptor;
    CryptoPP::CipherModeBase *decryptor;
    encryptor = CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    decryptor = CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);

    if (!isKeyValid(getThis(), encryptor)) {
        RETURN_FALSE;
    }

    // set the key on both the php object and the native cryptopp object
    encryptor->SetKey(reinterpret_cast<byte*>(key), keySize);
    decryptor->SetKey(reinterpret_cast<byte*>(key), keySize);
    zend_update_property_stringl(cryptopp_ce_MacAbstract, getThis(), "key", 3, key, keySize TSRMLS_CC);
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
