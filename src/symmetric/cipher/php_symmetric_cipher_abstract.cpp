#include "../../php_cryptopp.h"
#include "php_symmetric_cipher_abstract.h"
#include "php_symmetric_cipher_interface.h"
#include "php_symmetric_cipher.h"
#include "../../exception/php_exception.h"
#include <zend_exceptions.h>
#include <string>

/* {{{ custom object create/free handler */
zend_object_handlers SymmetricCipherAbstract_object_handlers;

void SymmetricCipherAbstract_free_storage(void *object TSRMLS_DC) {
    SymmetricCipherAbstractContainer *obj = static_cast<SymmetricCipherAbstractContainer *>(object);
    delete obj->encryptor;
    delete obj->decryptor;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}

zend_object_value SymmetricCipherAbstract_create_handler(zend_class_entry *type TSRMLS_DC) {
    zend_object_value retval;

    SymmetricCipherAbstractContainer *obj = static_cast<SymmetricCipherAbstractContainer *>(emalloc(sizeof(SymmetricCipherAbstractContainer)));
    memset(obj, 0, sizeof(SymmetricCipherAbstractContainer));

    zend_object_std_init(&obj->std, type TSRMLS_CC);

    #if PHP_VERSION_ID < 50399
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    #else
        object_properties_init(static_cast<zend_object*>(&(obj->std)), type);
    #endif

    retval.handle   = zend_objects_store_put(obj, NULL, SymmetricCipherAbstract_free_storage, NULL TSRMLS_CC);
    retval.handlers = &SymmetricCipherAbstract_object_handlers;

    return retval;
}
/* }}} */

/* {{{ PHP abstract class declaration */
zend_class_entry *cryptopp_ce_SymmetricCipherAbstract;

static zend_function_entry cryptopp_methods_SymmetricCipherAbstract[] = {
    PHP_ME(Cryptopp_SymmetricCipherAbstract, __sleep, arginfo_SymmetricCipherAbstract___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricCipherAbstract, __wakeup, arginfo_SymmetricCipherAbstract___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_SymmetricCipherAbstract, getName, arginfo_SymmetricCipherInterface_getName, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_SymmetricCipherAbstract(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "SymmetricCipherAbstract", cryptopp_methods_SymmetricCipherAbstract);
    cryptopp_ce_SymmetricCipherAbstract             = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_SymmetricCipherAbstract->ce_flags  |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

    cryptopp_ce_SymmetricCipherAbstract->create_object = SymmetricCipherAbstract_create_handler;
    memcpy(&SymmetricCipherAbstract_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    SymmetricCipherAbstract_object_handlers.clone_obj = NULL;

    zend_class_implements(cryptopp_ce_SymmetricCipherAbstract TSRMLS_CC, 1, cryptopp_ce_SymmetricCipherInterface);

    zend_declare_property_string(cryptopp_ce_SymmetricCipherAbstract, "name", 4, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ inits a child class */
void init_class_SymmetricCipherAbstractChild(const char *algoName, const char* className, zend_class_entry *classEntryPtr, zend_function_entry *classMethods TSRMLS_DC) {
    std::string namespacedClassName("Cryptopp\\");
    namespacedClassName.append(className);

    zend_class_entry ce;
    INIT_CLASS_ENTRY_EX(ce, namespacedClassName.c_str(), namespacedClassName.length(), classMethods);
    classEntryPtr = zend_register_internal_class_ex(&ce, cryptopp_ce_SymmetricCipherAbstract, NULL TSRMLS_CC);

    addSymmetricCipherAlgo(algoName, namespacedClassName.c_str());
}
/* }}} */

/* {{{ get the pointer to the native encryptor object of a php cipher class */
CryptoPP::BlockCipher *getCryptoppSymmetricCipherEncryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::BlockCipher *encryptor;
    encryptor = static_cast<SymmetricCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->encryptor;

    if (NULL == encryptor) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\SymmetricCipherAbstract cannot be extended by user classes");
    }

    return encryptor;
}
/* }}} */

/* {{{ get the pointer to the native decryptor object of a php cipher class */
CryptoPP::BlockCipher *getCryptoppSymmetricCipherDecryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::BlockCipher *decryptor;
    decryptor = static_cast<SymmetricCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->decryptor;

    if (NULL == decryptor) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\SymmetricCipherAbstract cannot be extended by user classes");
    }

    return decryptor;
}
/* }}} */

/* {{{ set the pointer to the native encryptor object of a php cipher class */
void setCryptoppSymmetricCipherEncryptorPtr(zval *this_ptr, CryptoPP::BlockCipher *encryptorPtr TSRMLS_DC) {
    static_cast<SymmetricCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->encryptor = encryptorPtr;
}
/* }}} */

/* {{{ set the pointer to the native decryptor object of a php cipher class */
void setCryptoppSymmetricCipherDecryptorPtr(zval *this_ptr, CryptoPP::BlockCipher *decryptorPtr TSRMLS_DC) {
    static_cast<SymmetricCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->decryptor = decryptorPtr;
}
/* }}} */

/* {{{ proto void SymmetricCipherAbstract::__sleep(void)
   Prevents serialization of a SymmetricCipherAbstract instance */
PHP_METHOD(Cryptopp_SymmetricCipherAbstract, __sleep) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\SymmetricCipherAbstract instances");
}
/* }}} */

/* {{{ proto void SymmetricCipherAbstract::__wakeup(void)
   Prevents use of a SymmetricCipherAbstract instance that has been unserialized */
PHP_METHOD(Cryptopp_SymmetricCipherAbstract, __wakeup) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\SymmetricCipherAbstract instances");
}
/* }}} */

/* {{{ proto string SymmetricCipherAbstract::getName(void)
   Return algorithm name */
PHP_METHOD(Cryptopp_SymmetricCipherAbstract, getName) {
    CryptoPP::BlockCipher *encryptor;
    encryptor = CRYPTOPP_SYMMETRIC_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)

    zval *name;
    name = zend_read_property(cryptopp_ce_SymmetricCipherAbstract, getThis(), "name", 4, 0 TSRMLS_CC);
    RETURN_ZVAL(name, 1, 0);
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
