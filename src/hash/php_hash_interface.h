#ifndef HASH_INTERFACE_H
#define HASH_INTERFACE_H

#include "../php_cryptopp.h"
#include "php_hash.h"

/* inits a php hash class */
#define CRYPTOPP_HASH_INIT_CLASS(algoName, classname, classEntryPtrName, classMethodsVarName) \
    CRYPTOPP_HASH_INIT_CLASS_FUNC_HEADER(classname) {                                       \
        zend_class_entry ce;                                                                \
        INIT_NS_CLASS_ENTRY(ce, PHP_CRYPTOPP_NAMESPACE, #classname, classMethodsVarName);   \
        classEntryPtrName = zend_register_internal_class(&ce TSRMLS_CC);                    \
                                                                                            \
        classEntryPtrName->create_object = HashInterface_create_handler;                    \
        memcpy(&HashInterface_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
        HashInterface_object_handlers.clone_obj = NULL;                                     \
                                                                                            \
        zend_class_implements(classEntryPtrName TSRMLS_CC, 1, cryptopp_ce_HashInterface);   \
                                                                                            \
        addHashAlgo(algoName, PHP_CRYPTOPP_NAMESPACE "\\" #classname);                      \
    }

/* header of the function that init a php hash class */
#define CRYPTOPP_HASH_INIT_CLASS_FUNC_HEADER(classname) void init_class_ ## classname(TSRMLS_D)

/* call the function that init a php hash class */
#define CRYPTOPP_HASH_INIT_CLASS_FUNC_CALL(classname) init_class_ ## classname(TSRMLS_C);

/* get the pointer to the native hash object of a php hash class */
#define CRYPTOPP_HASH_GET_NATIVE_PTR(classname) ((HashInterfaceContainer *)zend_object_store_get_object(getThis() TSRMLS_CC))->hash

/* set the pointer to the native hash object of a php hash class */
#define CRYPTOPP_HASH_SET_NATIVE_PTR(classname, nativeHashPtr) ((HashInterfaceContainer *)zend_object_store_get_object(getThis() TSRMLS_CC))->hash = nativeHashPtr;

/* php hash classes required methods declarations */
#define CRYPTOPP_HASH_REQUIRED_METHODS(classname)                                                                       \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, getName, arginfo_HashInterface_getName, ZEND_ACC_PUBLIC)               \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, getDigestSize, arginfo_HashInterface_getDigestSize, ZEND_ACC_PUBLIC)   \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, calculateDigest, arginfo_HashInterface_calculateDigest, ZEND_ACC_PUBLIC)

/* php hash classes required methods declarations to include in the headers */
#define CRYPTOPP_HASH_REQUIRED_METHODS_HEADER(classname)                \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, getDigestSize);    \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, getName);          \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, calculateDigest);

/* php hash classes common methods body */
#define CRYPTOPP_HASH_COMMON_METHODS_DEFINITIONS(classname, nativeClassname)        \
    /* {{{ proto int getDigestSize()                                                \
       Returns the digest size */                                                   \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, getDigestSize) {               \
        RETURN_LONG(nativeClassname::DIGESTSIZE);                                   \
    }                                                                               \
    /* }}} */                                                                       \
                                                                                    \
    /* {{{ proto string calculateDigest(data)                                       \
       Calculate the digest */                                                      \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, calculateDigest) {             \
        HashInterface_calculateDigest(INTERNAL_FUNCTION_PARAM_PASSTHRU);            \
    }
    /* }}} */                                                                       \

/* {{{ php hash classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_getName, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_getDigestSize, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_calculateDigest, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()
/* }}} */

/* pointer to the HashInterface zend class entry */
extern zend_class_entry *cryptopp_ce_HashInterface;

/* inits the HashInterface php interface */
void init_interface_HashInterface(TSRMLS_D);

/* {{{ object creation related stuff */
struct HashInterfaceContainer {
    zend_object std;
    CryptoPP::HashTransformation *hash;
};

extern zend_object_handlers HashInterface_object_handlers;
void HashInterface_free_storage(void *object TSRMLS_DC);
zend_object_value HashInterface_create_handler(zend_class_entry *type TSRMLS_DC);
/* }}} */

/* {{{ hash common methods */
void HashInterface_calculateDigest(INTERNAL_FUNCTION_PARAMETERS);
/* }}} */

#endif /* HASH_INTERFACE_H */
