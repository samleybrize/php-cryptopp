#ifndef PHP_RBG_INTERFACE_H
#define PHP_RBG_INTERFACE_H

#include "../php_cryptopp.h"
#include <zend_exceptions.h>

/* inits a php rbg class */
#define CRYPTOPP_RBG_INIT_CLASS(classname, classEntryPtrName, classMethodsVarName)          \
    CRYPTOPP_RBG_INIT_CLASS_FUNC_HEADER(classname) {                                        \
        zend_class_entry ce;                                                                \
        INIT_NS_CLASS_ENTRY(ce, PHP_CRYPTOPP_NAMESPACE, #classname, classMethodsVarName);   \
        classEntryPtrName = zend_register_internal_class(&ce TSRMLS_CC);                    \
                                                                                            \
        classEntryPtrName->create_object = RandomByteGeneratorInterface_create_handler;     \
        memcpy(&RandomByteGeneratorInterface_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
        RandomByteGeneratorInterface_object_handlers.clone_obj = NULL;                      \
                                                                                            \
        zend_class_implements(classEntryPtrName TSRMLS_CC, 1, cryptopp_ce_RandomByteGeneratorInterface);   \
    }

/* header of the function that init a php rbg class */
#define CRYPTOPP_RBG_INIT_CLASS_FUNC_HEADER(classname) void init_class_ ## classname(TSRMLS_D)

/* call the function that init a php rbg class */
#define CRYPTOPP_RBG_INIT_CLASS_FUNC_CALL(classname) init_class_ ## classname(TSRMLS_C);

/* get the pointer to the native rbg object of a php rbg class */
#define CRYPTOPP_RBG_GET_NATIVE_PTR(classname) ((RandomByteGeneratorInterfaceContainer *)zend_object_store_get_object(getThis() TSRMLS_CC))->rbg

/* set the pointer to the native rbg object of a php rbg class */
#define CRYPTOPP_RBG_SET_NATIVE_PTR(classname, nativeRandomByteGeneratorPtr) ((RandomByteGeneratorInterfaceContainer *)zend_object_store_get_object(getThis() TSRMLS_CC))->rbg = nativeRandomByteGeneratorPtr;

/* php rbg classes required methods declarations */
#define CRYPTOPP_RBG_REQUIRED_METHODS(classname)                                                                           \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, __sleep, arginfo_RandomByteGeneratorInterface___sleep, ZEND_ACC_PUBLIC)   \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, __wakeup, arginfo_RandomByteGeneratorInterface___wakeup, ZEND_ACC_PUBLIC) \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, generate, arginfo_RandomByteGeneratorInterface_generate, ZEND_ACC_PUBLIC)

/* php rbg classes required methods declarations to include in the headers */
#define CRYPTOPP_RBG_REQUIRED_METHODS_HEADER(classname)         \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, __sleep);  \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, __wakeup); \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, generate);

/* php rbg classes common methods body */
#define CRYPTOPP_RBG_COMMON_METHODS_DEFINITIONS(classname, nativeClassname)         \
    /* {{{ proto string RandomByteGeneratorInterface::__sleep(void)                 \
       Prevents serialization of a RandomByteGeneratorInterface instance */         \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, __sleep) {                     \
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, "You cannot serialize or unserialize " #classname " instances"); \
    }                                                                               \
    /* }}} */                                                                       \
                                                                                    \
    /* {{{ proto string RandomByteGeneratorInterface::__wakeup(void)                \
       Prevents use of a RandomByteGeneratorInterface instance that has been unserialized */ \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, __wakeup) {                    \
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, "You cannot serialize or unserialize " #classname " instances"); \
    }                                                                               \
    /* }}} */                                                                       \
                                                                                    \
    /* {{{ proto string RandomByteGeneratorInterface::generate(int size)            \
       generate random byte sequence */                                             \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, generate) {                    \
        RandomByteGeneratorInterface_generate(INTERNAL_FUNCTION_PARAM_PASSTHRU);    \
    }
    /* }}} */                                                                       \

/* {{{ php rbg classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_RandomByteGeneratorInterface___sleep, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_RandomByteGeneratorInterface___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_RandomByteGeneratorInterface_generate, 0)
    ZEND_ARG_INFO(0, size)
ZEND_END_ARG_INFO()
/* }}} */

/* pointer to the RandomByteGeneratorInterface zend class entry */
extern zend_class_entry *cryptopp_ce_RandomByteGeneratorInterface;

/* inits the RandomByteGeneratorInterface php interface */
void init_interface_RandomByteGeneratorInterface(TSRMLS_D);

/* {{{ object creation related stuff */
struct RandomByteGeneratorInterfaceContainer {
    zend_object std;
    CryptoPP::RandomNumberGenerator *rbg;
};

extern zend_object_handlers RandomByteGeneratorInterface_object_handlers;
void RandomByteGeneratorInterface_free_storage(void *object TSRMLS_DC);
zend_object_value RandomByteGeneratorInterface_create_handler(zend_class_entry *type TSRMLS_DC);
/* }}} */

/* {{{ rbg common methods */
void RandomByteGeneratorInterface_generate(INTERNAL_FUNCTION_PARAMETERS);
/* }}} */

#endif /* PHP_RBG_INTERFACE_H */
