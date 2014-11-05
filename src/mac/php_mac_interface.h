#ifndef PHP_MAC_INTERFACE_H
#define PHP_MAC_INTERFACE_H

#include "../php_cryptopp.h"
#include <zend_exceptions.h>

/* inits a php Mac class */
#define CRYPTOPP_MAC_INIT_CLASS(classname, classEntryPtrName, classMethodsVarName)          \
    CRYPTOPP_MAC_INIT_CLASS_FUNC_HEADER(classname) {                                        \
        zend_class_entry ce;                                                                \
        INIT_NS_CLASS_ENTRY(ce, PHP_CRYPTOPP_NAMESPACE, #classname, classMethodsVarName);   \
        classEntryPtrName = zend_register_internal_class(&ce TSRMLS_CC);                    \
                                                                                            \
        classEntryPtrName->create_object = MacInterface_create_handler;                     \
        memcpy(&MacInterface_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
        MacInterface_object_handlers.clone_obj = NULL;                                      \
                                                                                            \
        zend_class_implements(classEntryPtrName TSRMLS_CC, 1, cryptopp_ce_MacInterface);    \
                                                                                            \
        zend_declare_property_string(classEntryPtrName, "key", 3, "",  ZEND_ACC_PROTECTED TSRMLS_CC);  \
    }

/* header of the function that init a php Mac class */
#define CRYPTOPP_MAC_INIT_CLASS_FUNC_HEADER(classname) void init_class_ ## classname(TSRMLS_D)

/* call the function that init a php Mac class */
#define CRYPTOPP_MAC_INIT_CLASS_FUNC_CALL(classname) init_class_ ## classname(TSRMLS_C);

/* get the pointer to the native Mac object of a php Mac class */
#define CRYPTOPP_MAC_GET_NATIVE_PTR(classname) ((MacInterfaceContainer *)zend_object_store_get_object(getThis() TSRMLS_CC))->mac

/* set the pointer to the native Mac object of a php Mac class */
#define CRYPTOPP_MAC_SET_NATIVE_PTR(classname, nativeMacPtr) ((MacInterfaceContainer *)zend_object_store_get_object(getThis() TSRMLS_CC))->mac = nativeMacPtr;

/* php Mac classes required methods declarations */
#define CRYPTOPP_MAC_REQUIRED_METHODS(classname)                                                            \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, __sleep, arginfo_MacInterface___sleep, ZEND_ACC_PUBLIC)    \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, __wakeup, arginfo_MacInterface___wakeup, ZEND_ACC_PUBLIC)  \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, getName, arginfo_MacInterface_getName, ZEND_ACC_PUBLIC)    \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, getDigestSize, arginfo_MacInterface_getDigestSize, ZEND_ACC_PUBLIC)  \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, setKey, arginfo_MacInterface_setKey, ZEND_ACC_PUBLIC)      \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, calculateDigest, arginfo_MacInterface_calculateDigest, ZEND_ACC_PUBLIC)  \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, update, arginfo_MacInterface_update, ZEND_ACC_PUBLIC)      \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, final, arginfo_MacInterface_final, ZEND_ACC_PUBLIC)        \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, restart, arginfo_MacInterface_restart, ZEND_ACC_PUBLIC)

/* php Mac classes required methods declarations to include in the headers */
#define CRYPTOPP_MAC_REQUIRED_METHODS_HEADER(classname)         \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, __sleep);  \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, __wakeup); \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, getName);  \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, getDigestSize);  \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, setKey);   \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, calculateDigest);  \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, update);   \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, final);    \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, restart);

/* php Mac classes common methods bodies */
#define CRYPTOPP_MAC_COMMON_METHODS_DEFINITIONS(classname, nativeClassname)         \
    /* {{{ proto string MacInterface::__sleep(void)                                 \
       Prevents serialization of a MacInterface instance */                         \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, __sleep) {                     \
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize " #classname " instances"); \
    }                                                                               \
    /* }}} */                                                                       \
                                                                                    \
    /* {{{ proto string MacInterface::__wakeup(void)                                \
       Prevents use of a MacInterface instance that has been unserialized */        \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, __wakeup) {                    \
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize " #classname " instances"); \
    }                                                                               \
    /* }}} */                                                                       \
                                                                                    \
    /* {{{ proto string MacInterface::getDigestSize(void)                           \
       Returns the digest size */                                                   \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, getDigestSize) {               \
        RETURN_LONG(nativeClassname::DIGESTSIZE);                                   \
    }                                                                               \
    /* }}} */                                                                       \
                                                                                    \
    /* {{{ proto string MacInterface::setKey(string key)                            \
       Sets the key */                                                              \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, setKey) {                      \
        MacInterface_setKey(INTERNAL_FUNCTION_PARAM_PASSTHRU, cryptopp_ce_ ## classname);  \
    }                                                                               \
    /* }}} */                                                                       \
                                                                                    \
    /* {{{ proto string MacInterface::calculateDigest(string data)                  \
       Calculate the MAC */                                                         \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, calculateDigest) {             \
        MacInterface_calculateDigest(INTERNAL_FUNCTION_PARAM_PASSTHRU);             \
    }                                                                               \
    /* }}} */                                                                       \
                                                                                    \
    /* {{{ proto string MacInterface::update(string data)                           \
       Adds data to the current incremental MAC */                                  \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, update) {                      \
        MacInterface_update(INTERNAL_FUNCTION_PARAM_PASSTHRU);                      \
    }                                                                               \
    /* }}} */                                                                       \
                                                                                    \
    /* {{{ proto string MacInterface::final(void)                                   \
       Finalize an incremental MAC and return resulting digest */                   \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, final) {                       \
        MacInterface_final(INTERNAL_FUNCTION_PARAM_PASSTHRU);                       \
    }                                                                               \
    /* }}} */                                                                       \
                                                                                    \
    /* {{{ proto string MacInterface::restart(void)                                 \
       Discard the current incremental MAC */                                       \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, restart) {                     \
        MacInterface_restart(INTERNAL_FUNCTION_PARAM_PASSTHRU);                     \
    }                                                                               \
    /* }}} */

/* {{{ php Mac classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_MacInterface___sleep, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_MacInterface___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_MacInterface_getName, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_MacInterface_getDigestSize, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_MacInterface_setKey, 0)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_MacInterface_calculateDigest, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_MacInterface_update, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_MacInterface_final, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_MacInterface_restart, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* pointer to the MacInterface zend class entry */
extern zend_class_entry *cryptopp_ce_MacInterface;

/* inits the MacInterface php interface */
void init_interface_MacInterface(TSRMLS_D);

/* {{{ object creation related stuff */
struct MacInterfaceContainer {
    zend_object std;
    CryptoPP::MessageAuthenticationCode *mac;
};

extern zend_object_handlers MacInterface_object_handlers;
void MacInterface_free_storage(void *object TSRMLS_DC);
zend_object_value MacInterface_create_handler(zend_class_entry *type TSRMLS_DC);
/* }}} */

/* {{{ Mac common methods */
void MacInterface_setKey(INTERNAL_FUNCTION_PARAMETERS, zend_class_entry *ce);
void MacInterface_calculateDigest(INTERNAL_FUNCTION_PARAMETERS);
void MacInterface_update(INTERNAL_FUNCTION_PARAMETERS);
void MacInterface_final(INTERNAL_FUNCTION_PARAMETERS);
void MacInterface_restart(INTERNAL_FUNCTION_PARAMETERS);
/* }}} */

#endif /* PHP_MAC_INTERFACE_H */
