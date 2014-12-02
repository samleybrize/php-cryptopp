#ifndef PHP_SYMMETRIC_MODE_INTERFACE_H
#define PHP_SYMMETRIC_MODE_INTERFACE_H

#include "../../php_cryptopp.h"
#include "php_symmetric_mode.h"
#include <modes.h>
#include <zend_exceptions.h>

/* {{{ inits a php symmetric mode class */
#define CRYPTOPP_SYMMETRIC_MODE_INIT_CLASS(algoName, classname, classEntryPtrName, classMethodsVarName) \
    CRYPTOPP_SYMMETRIC_MODE_INIT_CLASS_FUNC_HEADER(classname) {                             \
        zend_class_entry ce;                                                                \
        INIT_NS_CLASS_ENTRY(ce, "Cryptopp", #classname, classMethodsVarName);               \
        classEntryPtrName = zend_register_internal_class(&ce TSRMLS_CC);                    \
                                                                                            \
        classEntryPtrName->create_object = SymmetricModeInterface_create_handler;           \
        memcpy(&SymmetricModeInterface_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
        SymmetricModeInterface_object_handlers.clone_obj = NULL;                            \
                                                                                            \
        zend_class_implements(classEntryPtrName TSRMLS_CC, 1, cryptopp_ce_SymmetricModeInterface);   \
        zend_class_implements(classEntryPtrName TSRMLS_CC, 1, cryptopp_ce_SymmetricModeInternalInterface); \
                                                                                            \
        addSymmetricMode(algoName, "Cryptopp\\" #classname);                                \
                                                                                            \
        zend_declare_property_string(classEntryPtrName, "cipher", 6, "",  ZEND_ACC_PRIVATE TSRMLS_CC);  \
        zend_declare_property_string(classEntryPtrName, "name", 4, "",  ZEND_ACC_PRIVATE TSRMLS_CC);    \
        zend_declare_property_string(classEntryPtrName, "key", 3, "",  ZEND_ACC_PRIVATE TSRMLS_CC);     \
    }
/* }}} */

/* header of the function that init a php symmetric mode class */
#define CRYPTOPP_SYMMETRIC_MODE_INIT_CLASS_FUNC_HEADER(classname) void init_class_ ## classname(TSRMLS_D)

/* call the function that init a php symmetric mode class */
#define CRYPTOPP_SYMMETRIC_MODE_INIT_CLASS_FUNC_CALL(classname) init_class_ ## classname(TSRMLS_C);

/* get the pointer to the native symmetric mode object of a php symmetric mode class */
#define CRYPTOPP_SYMMETRIC_MODE_GET_ENCRYPTOR_PTR() static_cast<SymmetricModeInterfaceContainer *>(zend_object_store_get_object(getThis() TSRMLS_CC))->encryptor
#define CRYPTOPP_SYMMETRIC_MODE_GET_DECRYPTOR_PTR() static_cast<SymmetricModeInterfaceContainer *>(zend_object_store_get_object(getThis() TSRMLS_CC))->decryptor

/* set the pointer to the native symmetric mode object of a php symmetric mode class */
#define CRYPTOPP_SYMMETRIC_MODE_SET_ENCRYPTOR_PTR(nativeModePtr) static_cast<SymmetricModeInterfaceContainer *>(zend_object_store_get_object(getThis() TSRMLS_CC))->encryptor = nativeModePtr;
#define CRYPTOPP_SYMMETRIC_MODE_SET_DECRYPTOR_PTR(nativeModePtr) static_cast<SymmetricModeInterfaceContainer *>(zend_object_store_get_object(getThis() TSRMLS_CC))->decryptor = nativeModePtr;

/* {{{ php symmetric mode classes required methods declarations */
#define CRYPTOPP_SYMMETRIC_MODE_REQUIRED_METHODS(classname)                                                                 \
    PHP_ME(Cryptopp_ ## classname, __sleep, arginfo_SymmetricModeInterface___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)       \
    PHP_ME(Cryptopp_ ## classname, __wakeup, arginfo_SymmetricModeInterface___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)     \
    PHP_ME(Cryptopp_ ## classname, getName, arginfo_SymmetricModeInterface_getName, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)       \
    PHP_ME(Cryptopp_ ## classname, setKey, arginfo_SymmetricModeInterface_setKey, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)         \
/* }}} */

/* {{{ php symmetric mode classes required methods declarations to include in the headers */
#define CRYPTOPP_SYMMETRIC_MODE_REQUIRED_METHODS_HEADER(classname)  \
    PHP_METHOD(Cryptopp_ ## classname, __wakeup);       \
    PHP_METHOD(Cryptopp_ ## classname, __sleep);        \
    PHP_METHOD(Cryptopp_ ## classname, getName);        \
    PHP_METHOD(Cryptopp_ ## classname, setKey);         \
/* }}} */

/* {{{ php symmetric mode classes common methods bodies */
#define CRYPTOPP_SYMMETRIC_MODE_COMMON_METHODS_DEFINITIONS(classname, nativeClassname)          \
    /* {{{ proto void SymmetricModeInterface::__sleep(void)                                     \
       Prevents serialization of a SymmetricModeInterface instance */                           \
    PHP_METHOD(Cryptopp_ ## classname, __sleep) {                                               \
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize " #classname " instances"); \
    }                                                                                           \
    /* }}} */                                                                                   \
                                                                                                \
    /* {{{ proto void SymmetricModeInterface::__wakeup(void)                                    \
       Prevents use of a SymmetricModeInterface instance that has been unserialized */          \
    PHP_METHOD(Cryptopp_ ## classname, __wakeup) {                                              \
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize " #classname " instances"); \
    }                                                                                           \
    /* }}} */                                                                                   \
                                                                                                \
    /* {{{ proto void SymmetricModeInterface::setKey(string key)                                \
       Sets the key */                                                                          \
    PHP_METHOD(Cryptopp_ ## classname, setKey) {                                                \
        SymmetricModeInterface_setKey(INTERNAL_FUNCTION_PARAM_PASSTHRU);                        \
    }                                                                                           \
    /* }}} */
/* }}} */

/* {{{ php symmetric mode classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_SymmetricModeInterface___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricModeInterface___sleep, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricModeInterface_getName, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricModeInterface_setKey, 0)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()
/* }}} */

/* pointer to the SymmetricModeInterface zend class entry */
extern zend_class_entry *cryptopp_ce_SymmetricModeInterface;
extern zend_class_entry *cryptopp_ce_SymmetricModeInternalInterface;

/* inits the SymmetricModeInterface php interface */
void init_interface_SymmetricModeInterface(TSRMLS_D);

/* return a pointer to the SymmetricModeInterface class entry */
zend_class_entry *getCryptoppSymmetricModeInterface();
zend_class_entry *getCryptoppSymmetricModeInternalInterface();

/* {{{ object creation related stuff */
struct SymmetricModeInterfaceContainer {
    zend_object std;
    CryptoPP::CipherModeBase *encryptor;
    CryptoPP::CipherModeBase *decryptor;
};

extern zend_object_handlers SymmetricModeInterface_object_handlers;
void SymmetricModeInterface_free_storage(void *object TSRMLS_DC);
zend_object_value SymmetricModeInterface_create_handler(zend_class_entry *type TSRMLS_DC);
/* }}} */

/* {{{ Get needed cipher elements to build a mode object */
bool cryptoppSymmetricModeGetCipherElements(
    zval *cipherObject,
    zval *modeObject,
    CryptoPP::BlockCipher **cipherEncryptor,
    CryptoPP::BlockCipher **cipherDecryptor,
    std::string **cipherName
);
/* }}} */

/* {{{ Modes common methods */
void SymmetricModeInterface_setKey(INTERNAL_FUNCTION_PARAMETERS);
/* }}} */

#endif /* PHP_SYMMETRIC_MODE_INTERFACE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
