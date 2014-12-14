#include "../php_cryptopp.h"
#include "../exception/php_exception.h"
#include "php_hash.h"
#include "php_hash_transformation_interface.h"
#include "php_hash_interface.h"
#include "php_hash_abstract.h"
#include <zend_exceptions.h>
#include <string>

/* {{{ custom object create/free handler */
zend_object_handlers HashAbstract_object_handlers;

void HashAbstract_free_storage(void *object TSRMLS_DC) {
    HashAbstractContainer *obj = static_cast<HashAbstractContainer *>(object);
    delete obj->hash;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}

zend_object_value HashAbstract_create_handler(zend_class_entry *type TSRMLS_DC) {
    zend_object_value retval;

    HashAbstractContainer *obj = static_cast<HashAbstractContainer *>(emalloc(sizeof(HashAbstractContainer)));
    memset(obj, 0, sizeof(HashAbstractContainer));

    zend_object_std_init(&obj->std, type TSRMLS_CC);

    #if PHP_VERSION_ID < 50399
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    #else
        object_properties_init(static_cast<zend_object*>(&(obj->std)), type);
    #endif

    retval.handle   = zend_objects_store_put(obj, NULL, HashAbstract_free_storage, NULL TSRMLS_CC);
    retval.handlers = &HashAbstract_object_handlers;

    return retval;
}
/* }}} */

/* {{{ PHP abstract class declaration */
zend_class_entry *cryptopp_ce_HashAbstract;

static zend_function_entry cryptopp_methods_HashAbstract[] = {
    PHP_ME(Cryptopp_HashAbstract, __sleep, arginfo_HashAbstract___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_HashAbstract, __wakeup, arginfo_HashAbstract___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_HashAbstract, getName, arginfo_HashTransformationInterface_getName, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_HashAbstract, getDigestSize, arginfo_HashTransformationInterface_getDigestSize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_HashAbstract, getBlockSize, arginfo_HashInterface_getBlockSize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_HashAbstract, calculateDigest, arginfo_HashTransformationInterface_calculateDigest, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_HashAbstract, update, arginfo_HashTransformationInterface_update, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_HashAbstract, finalize, arginfo_HashTransformationInterface_finalize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_HashAbstract, restart, arginfo_HashTransformationInterface_restart, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_HashAbstract(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "HashAbstract", cryptopp_methods_HashAbstract);
    cryptopp_ce_HashAbstract            = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_HashAbstract->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

    cryptopp_ce_HashAbstract->create_object = HashAbstract_create_handler;
    memcpy(&HashAbstract_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    HashAbstract_object_handlers.clone_obj = NULL;

    zend_class_implements(cryptopp_ce_HashAbstract TSRMLS_CC, 1, cryptopp_ce_HashInterface);

    zend_declare_property_string(cryptopp_ce_HashAbstract, "name", 4, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ inits a child class */
void init_class_HashAbstractChild(const char *algoName, const char* className, zend_class_entry **classEntryPtr, zend_function_entry *classMethods TSRMLS_DC) {
    std::string namespacedClassName("Cryptopp\\");
    namespacedClassName.append(className);

    zend_class_entry ce;
    INIT_CLASS_ENTRY_EX(ce, namespacedClassName.c_str(), namespacedClassName.length(), classMethods);
    *classEntryPtr = zend_register_internal_class_ex(&ce, cryptopp_ce_HashAbstract, NULL TSRMLS_CC);

    addHashAlgo(algoName, namespacedClassName.c_str());
}
/* }}} */

/* {{{ get the pointer to the native hash object of a php hash class */
CryptoPP::HashTransformation *getCryptoppHashNativePtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::HashTransformation *hash;
    hash = static_cast<HashAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->hash;

    if (NULL == hash) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\HashAbstract cannot be extended by user classes");
    }

    return hash;
}
/* }}} */

/* {{{ set the pointer to the native hash object of a php hash class */
void setCryptoppHashNativePtr(zval *this_ptr, CryptoPP::HashTransformation *nativePtr TSRMLS_DC) {
    static_cast<HashAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->hash = nativePtr;
}
/* }}} */

/* {{{ proto void HashAbstract::__sleep(void)
   Prevents serialization of a HashAbstract instance */
PHP_METHOD(Cryptopp_HashAbstract, __sleep) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\HashAbstract instances");
}
/* }}} */

/* {{{ proto void HashAbstract::__wakeup(void)
   Prevents use of a HashAbstract instance that has been unserialized */
PHP_METHOD(Cryptopp_HashAbstract, __wakeup) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\HashAbstract instances");
}
/* }}} */

/* {{{ proto int HashAbstract::getDigestSize(void)
   Returns the digest size */
PHP_METHOD(Cryptopp_HashAbstract, getDigestSize) {
    CryptoPP::HashTransformation *hash;
    hash = CRYPTOPP_HASH_ABSTRACT_GET_NATIVE_PTR(hash)

    unsigned int digestSize = hash->DigestSize();
    RETURN_LONG(digestSize);
}
/* }}} */

/* {{{ proto int HashAbstract::getBlockSize(void)
   Returns the block size */
PHP_METHOD(Cryptopp_HashAbstract, getBlockSize) {
    CryptoPP::HashTransformation *hash;
    hash = CRYPTOPP_HASH_ABSTRACT_GET_NATIVE_PTR(hash)

    unsigned int digestSize = hash->BlockSize();
    RETURN_LONG(digestSize);
}
/* }}} */

/* {{{ proto string HashAbstract::getName(void)
   Return algorithm name */
PHP_METHOD(Cryptopp_HashAbstract, getName) {
    CryptoPP::HashTransformation *hash;
    hash = CRYPTOPP_HASH_ABSTRACT_GET_NATIVE_PTR(hash)

    zval *name;
    name = zend_read_property(cryptopp_ce_HashAbstract, getThis(), "name", 4, 0 TSRMLS_CC);
    RETURN_ZVAL(name, 1, 0);
}
/* }}} */

/* {{{ proto string HashAbstract::calculateDigest(string data)
   Calculate the digest */
PHP_METHOD(Cryptopp_HashAbstract, calculateDigest) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::HashTransformation *hash;
    hash = CRYPTOPP_HASH_ABSTRACT_GET_NATIVE_PTR(hash)

    byte digest[hash->DigestSize()];
    hash->CalculateDigest(digest, reinterpret_cast<byte*>(msg), msgSize);

    RETVAL_STRINGL(reinterpret_cast<char*>(digest), hash->DigestSize(), 1);
}
/* }}} */

/* {{{ proto void HashAbstract::update(string data)
   Adds data to the current incremental hash */
PHP_METHOD(Cryptopp_HashAbstract, update) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::HashTransformation *hash;
    hash = CRYPTOPP_HASH_ABSTRACT_GET_NATIVE_PTR(hash)

    hash->Update(reinterpret_cast<byte*>(msg), msgSize);
}
/* }}} */

/* {{{ proto string HashAbstract::finalize(void)
   Finalize an incremental hash and return resulting digest */
PHP_METHOD(Cryptopp_HashAbstract, finalize) {
    CryptoPP::HashTransformation *hash;
    hash = CRYPTOPP_HASH_ABSTRACT_GET_NATIVE_PTR(hash)

    byte digest[hash->DigestSize()];
    hash->Final(digest);

    RETVAL_STRINGL(reinterpret_cast<char*>(digest), hash->DigestSize(), 1);
}
/* }}} */

/* {{{ proto void HashAbstract::restart(void)
   Discard the current incremental hash */
PHP_METHOD(Cryptopp_HashAbstract, restart) {
    CryptoPP::HashTransformation *hash;
    hash = CRYPTOPP_HASH_ABSTRACT_GET_NATIVE_PTR(hash)

    hash->Restart();
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
