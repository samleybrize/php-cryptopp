#ifndef PHP_SYMMETRIC_CIPHER_INTERFACE_H
#define PHP_SYMMETRIC_CIPHER_INTERFACE_H

#include "../../php_cryptopp.h"
#include "php_symmetric_cipher.h"

/* {{{ inits a php symmetric cipher class */
#define CRYPTOPP_SYMMETRIC_CIPHER_INIT_CLASS(algoName, classname, classEntryPtrName, classMethodsVarName) \
    CRYPTOPP_SYMMETRIC_CIPHER_INIT_CLASS_FUNC_HEADER(classname) {                           \
        zend_class_entry ce;                                                                \
        INIT_NS_CLASS_ENTRY(ce, "Cryptopp", #classname, classMethodsVarName);               \
        classEntryPtrName = zend_register_internal_class(&ce TSRMLS_CC);                    \
                                                                                            \
        classEntryPtrName->create_object = SymmetricCipherInterface_create_handler;         \
        memcpy(&SymmetricCipherInterface_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
        SymmetricCipherInterface_object_handlers.clone_obj = NULL;                          \
                                                                                            \
        zend_class_implements(classEntryPtrName TSRMLS_CC, 1, cryptopp_ce_SymmetricCipherInterface);   \
        zend_class_implements(classEntryPtrName TSRMLS_CC, 1, cryptopp_ce_SymmetricCipherInternalInterface); \
                                                                                            \
        addSymmetricCipherAlgo(algoName, "Cryptopp\\" #classname);                          \
                                                                                            \
        zend_declare_property_string(classEntryPtrName, "name", 4, "",  ZEND_ACC_PRIVATE TSRMLS_CC); \
    }
/* }}} */

/* header of the function that init a php symmetric cipher class */
#define CRYPTOPP_SYMMETRIC_CIPHER_INIT_CLASS_FUNC_HEADER(classname) void init_class_ ## classname(TSRMLS_D)

/* call the function that init a php symmetric cipher class */
#define CRYPTOPP_SYMMETRIC_CIPHER_INIT_CLASS_FUNC_CALL(classname) init_class_ ## classname(TSRMLS_C);

/* get the pointer to the native symmetric cipher object of a php symmetric cipher class */
#define CRYPTOPP_SYMMETRIC_CIPHER_GET_ENCRYPTOR_PTR() static_cast<SymmetricCipherInterfaceContainer *>(zend_object_store_get_object(getThis() TSRMLS_CC))->encryptor
#define CRYPTOPP_SYMMETRIC_CIPHER_GET_DECRYPTOR_PTR() static_cast<SymmetricCipherInterfaceContainer *>(zend_object_store_get_object(getThis() TSRMLS_CC))->decryptor

/* set the pointer to the native symmetric cipher object of a php symmetric cipher class */
#define CRYPTOPP_SYMMETRIC_CIPHER_SET_ENCRYPTOR_PTR(nativeCipherPtr) static_cast<SymmetricCipherInterfaceContainer *>(zend_object_store_get_object(getThis() TSRMLS_CC))->encryptor = nativeCipherPtr;
#define CRYPTOPP_SYMMETRIC_CIPHER_SET_DECRYPTOR_PTR(nativeCipherPtr) static_cast<SymmetricCipherInterfaceContainer *>(zend_object_store_get_object(getThis() TSRMLS_CC))->decryptor = nativeCipherPtr;

/* {{{ php symmetric cipher classes required methods declarations */
#define CRYPTOPP_SYMMETRIC_CIPHER_REQUIRED_METHODS(classname)                                                               \
    PHP_ME(Cryptopp_ ## classname, __sleep, arginfo_SymmetricCipherInterface___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)     \
    PHP_ME(Cryptopp_ ## classname, __wakeup, arginfo_SymmetricCipherInterface___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)   \
    PHP_ME(Cryptopp_ ## classname, getName, arginfo_SymmetricCipherInterface_getName, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
/* }}} */

/* {{{ php symmetric cipher classes required methods declarations to include in the headers */
#define CRYPTOPP_SYMMETRIC_CIPHER_REQUIRED_METHODS_HEADER(classname)  \
    PHP_METHOD(Cryptopp_ ## classname, __wakeup);       \
    PHP_METHOD(Cryptopp_ ## classname, __sleep);        \
    PHP_METHOD(Cryptopp_ ## classname, getName);
/* }}} */

/* {{{ php symmetric cipher classes common methods bodies */
#define CRYPTOPP_SYMMETRIC_CIPHER_COMMON_METHODS_DEFINITIONS(classname, nativeClassname)        \
    /* {{{ proto void SymmetricCipherInterface::__sleep(void)                                   \
       Prevents serialization of a SymmetricCipherInterface instance */                         \
    PHP_METHOD(Cryptopp_ ## classname, __sleep) {                                               \
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize " #classname " instances"); \
    }                                                                                           \
    /* }}} */                                                                                   \
                                                                                                \
    /* {{{ proto void SymmetricCipherInterface::__wakeup(void)                                  \
       Prevents use of a SymmetricCipherInterface instance that has been unserialized */        \
    PHP_METHOD(Cryptopp_ ## classname, __wakeup) {                                              \
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize " #classname " instances"); \
    }                                                                                           \
    /* }}} */                                                                                   \
/* }}} */

/* {{{ php symmetric cipher classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_SymmetricCipherInterface___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricCipherInterface___sleep, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricCipherInterface_getName, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* pointer to the SymmetricCipherInterface zend class entry */
extern zend_class_entry *cryptopp_ce_SymmetricCipherInterface;
extern zend_class_entry *cryptopp_ce_SymmetricCipherInternalInterface;

/* inits the SymmetricCipherInterface php interface */
void init_interface_SymmetricCipherInterface(TSRMLS_D);

/* return a pointer to the SymmetricCipherInterface class entry */
zend_class_entry *getCryptoppSymmetricCipherInterface();
zend_class_entry *getCryptoppSymmetricCipherInternalInterface();

/* {{{ object creation related stuff */
struct SymmetricCipherInterfaceContainer {
    zend_object std;
    CryptoPP::BlockCipher *encryptor;
    CryptoPP::BlockCipher *decryptor;
};

extern zend_object_handlers SymmetricCipherInterface_object_handlers;
void SymmetricCipherInterface_free_storage(void *object TSRMLS_DC);
zend_object_value SymmetricCipherInterface_create_handler(zend_class_entry *type TSRMLS_DC);
/* }}} */

#endif /* PHP_SYMMETRIC_CIPHER_INTERFACE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
