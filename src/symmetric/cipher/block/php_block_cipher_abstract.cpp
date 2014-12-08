#include "../../../php_cryptopp.h"
#include "../../../exception/php_exception.h"
#include "../php_symmetric_cipher_interface.h"
#include "php_block_cipher.h"
#include "php_block_cipher_interface.h"
#include "php_block_cipher_abstract.h"
#include <zend_exceptions.h>
#include <string>

/* {{{ custom object create/free handler */
zend_object_handlers BlockCipherAbstract_object_handlers;

void BlockCipherAbstract_free_storage(void *object TSRMLS_DC) {
    BlockCipherAbstractContainer *obj = static_cast<BlockCipherAbstractContainer *>(object);
    delete obj->encryptor;
    delete obj->decryptor;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}

zend_object_value BlockCipherAbstract_create_handler(zend_class_entry *type TSRMLS_DC) {
    zend_object_value retval;

    BlockCipherAbstractContainer *obj = static_cast<BlockCipherAbstractContainer *>(emalloc(sizeof(BlockCipherAbstractContainer)));
    memset(obj, 0, sizeof(BlockCipherAbstractContainer));

    zend_object_std_init(&obj->std, type TSRMLS_CC);

    #if PHP_VERSION_ID < 50399
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    #else
        object_properties_init(static_cast<zend_object*>(&(obj->std)), type);
    #endif

    retval.handle   = zend_objects_store_put(obj, NULL, BlockCipherAbstract_free_storage, NULL TSRMLS_CC);
    retval.handlers = &BlockCipherAbstract_object_handlers;

    return retval;
}
/* }}} */

/* {{{ PHP abstract class declaration */
zend_class_entry *cryptopp_ce_BlockCipherAbstract;

static zend_function_entry cryptopp_methods_BlockCipherAbstract[] = {
    PHP_ME(Cryptopp_BlockCipherAbstract, __sleep, arginfo_BlockCipherAbstract___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_BlockCipherAbstract, __wakeup, arginfo_BlockCipherAbstract___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_BlockCipherAbstract, getName, arginfo_SymmetricCipherInterface_getName, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_BlockCipherAbstract(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "BlockCipherAbstract", cryptopp_methods_BlockCipherAbstract);
    cryptopp_ce_BlockCipherAbstract             = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_BlockCipherAbstract->ce_flags  |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

    cryptopp_ce_BlockCipherAbstract->create_object = BlockCipherAbstract_create_handler;
    memcpy(&BlockCipherAbstract_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    BlockCipherAbstract_object_handlers.clone_obj = NULL;

    zend_class_implements(cryptopp_ce_BlockCipherAbstract TSRMLS_CC, 1, cryptopp_ce_BlockCipherInterface);

    zend_declare_property_string(cryptopp_ce_BlockCipherAbstract, "name", 4, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ inits a child class */
void init_class_BlockCipherAbstractChild(const char *algoName, const char* className, zend_class_entry *classEntryPtr, zend_function_entry *classMethods TSRMLS_DC) {
    std::string namespacedClassName("Cryptopp\\");
    namespacedClassName.append(className);

    zend_class_entry ce;
    INIT_CLASS_ENTRY_EX(ce, namespacedClassName.c_str(), namespacedClassName.length(), classMethods);
    classEntryPtr = zend_register_internal_class_ex(&ce, cryptopp_ce_BlockCipherAbstract, NULL TSRMLS_CC);

    addBlockCipherAlgo(algoName, namespacedClassName.c_str());
}
/* }}} */

/* {{{ get the pointer to the native encryptor object of a php cipher class */
CryptoPP::BlockCipher *getCryptoppBlockCipherEncryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::BlockCipher *encryptor;
    encryptor = static_cast<BlockCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->encryptor;

    if (NULL == encryptor) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\BlockCipherAbstract cannot be extended by user classes");
    }

    return encryptor;
}
/* }}} */

/* {{{ get the pointer to the native decryptor object of a php cipher class */
CryptoPP::BlockCipher *getCryptoppBlockCipherDecryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::BlockCipher *decryptor;
    decryptor = static_cast<BlockCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->decryptor;

    if (NULL == decryptor) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\BlockCipherAbstract cannot be extended by user classes");
    }

    return decryptor;
}
/* }}} */

/* {{{ set the pointer to the native encryptor object of a php cipher class */
void setCryptoppBlockCipherEncryptorPtr(zval *this_ptr, CryptoPP::BlockCipher *encryptorPtr TSRMLS_DC) {
    static_cast<BlockCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->encryptor = encryptorPtr;
}
/* }}} */

/* {{{ set the pointer to the native decryptor object of a php cipher class */
void setCryptoppBlockCipherDecryptorPtr(zval *this_ptr, CryptoPP::BlockCipher *decryptorPtr TSRMLS_DC) {
    static_cast<BlockCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->decryptor = decryptorPtr;
}
/* }}} */

/* {{{ proto void BlockCipherAbstract::__sleep(void)
   Prevents serialization of a BlockCipherAbstract instance */
PHP_METHOD(Cryptopp_BlockCipherAbstract, __sleep) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\BlockCipherAbstract instances");
}
/* }}} */

/* {{{ proto void BlockCipherAbstract::__wakeup(void)
   Prevents use of a BlockCipherAbstract instance that has been unserialized */
PHP_METHOD(Cryptopp_BlockCipherAbstract, __wakeup) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\BlockCipherAbstract instances");
}
/* }}} */

/* {{{ proto string BlockCipherAbstract::getName(void)
   Return algorithm name */
PHP_METHOD(Cryptopp_BlockCipherAbstract, getName) {
    CryptoPP::BlockCipher *encryptor;
    encryptor = CRYPTOPP_BLOCK_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)

    zval *name;
    name = zend_read_property(cryptopp_ce_BlockCipherAbstract, getThis(), "name", 4, 0 TSRMLS_CC);
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
