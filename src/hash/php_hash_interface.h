#ifndef PHP_HASH_INTERFACE_H
#define PHP_HASH_INTERFACE_H

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
#define CRYPTOPP_HASH_REQUIRED_METHODS(classname)                                                                           \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, __sleep, arginfo_HashInterface___sleep, ZEND_ACC_PUBLIC)                   \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, __wakeup, arginfo_HashInterface___wakeup, ZEND_ACC_PUBLIC)                 \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, getName, arginfo_HashInterface_getName, ZEND_ACC_PUBLIC)                   \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, getDigestSize, arginfo_HashInterface_getDigestSize, ZEND_ACC_PUBLIC)       \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, calculateDigest, arginfo_HashInterface_calculateDigest, ZEND_ACC_PUBLIC)   \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, update, arginfo_HashInterface_update, ZEND_ACC_PUBLIC)                     \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, final, arginfo_HashInterface_final, ZEND_ACC_PUBLIC)                       \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, restart, arginfo_HashInterface_restart, ZEND_ACC_PUBLIC)

/* php hash classes required methods declarations to include in the headers */
#define CRYPTOPP_HASH_REQUIRED_METHODS_HEADER(classname)                \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, __wakeup);         \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, __sleep);          \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, getName);          \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, getDigestSize);    \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, calculateDigest);  \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, update);           \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, final);            \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, restart);

/* php hash classes common methods body */
#define CRYPTOPP_HASH_COMMON_METHODS_DEFINITIONS(classname, nativeClassname)        \
    /* {{{ proto string HashInterface::__sleep(void)                                \
       Prevents serialization of a HashInterface instance */                        \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, __sleep) {                     \
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize " #classname " instances"); \
    }                                                                               \
    /* }}} */                                                                       \
                                                                                    \
    /* {{{ proto string HashInterface::__wakeup(void)                               \
       Prevents use of a HashInterface instance that has been unserialized */       \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, __wakeup) {                    \
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize " #classname " instances"); \
    }                                                                               \
    /* }}} */                                                                       \
                                                                                    \
    /* {{{ proto int HashInterface::getDigestSize(void)                             \
       Returns the digest size */                                                   \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, getDigestSize) {               \
        RETURN_LONG(nativeClassname::DIGESTSIZE);                                   \
    }                                                                               \
    /* }}} */                                                                       \
                                                                                    \
    /* {{{ proto string HashInterface::calculateDigest(string data)                 \
       Calculate the digest */                                                      \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, calculateDigest) {             \
        HashInterface_calculateDigest(INTERNAL_FUNCTION_PARAM_PASSTHRU);            \
    }                                                                               \
    /* }}} */                                                                       \
                                                                                    \
    /* {{{ proto string HashInterface::update(string data)                          \
       Adds data to the current incremental hash */                                 \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, update) {                      \
        HashInterface_update(INTERNAL_FUNCTION_PARAM_PASSTHRU);                     \
    }                                                                               \
    /* }}} */                                                                       \
                                                                                    \
    /* {{{ proto string HashInterface::final(void)                                  \
       Finalize an incremental hash and return resulting digest */                  \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, final) {                       \
        HashInterface_final(INTERNAL_FUNCTION_PARAM_PASSTHRU);                      \
    }                                                                               \
    /* }}} */                                                                       \
                                                                                    \
    /* {{{ proto string HashInterface::restart(void)                                \
       Discard the current incremental hash */                                      \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, restart) {                     \
        HashInterface_restart(INTERNAL_FUNCTION_PARAM_PASSTHRU);                    \
    }
    /* }}} */

/* {{{ php hash classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_HashInterface___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashInterface___sleep, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_getName, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_getDigestSize, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_calculateDigest, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_update, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_final, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_restart, 0)
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
void HashInterface_update(INTERNAL_FUNCTION_PARAMETERS);
void HashInterface_final(INTERNAL_FUNCTION_PARAMETERS);
void HashInterface_restart(INTERNAL_FUNCTION_PARAMETERS);
/* }}} */

#endif /* PHP_HASH_INTERFACE_H */
