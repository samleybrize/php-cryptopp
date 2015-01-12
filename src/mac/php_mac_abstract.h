#ifndef PHP_MAC_ABSTRACT_H
#define PHP_MAC_ABSTRACT_H

#include "../php_cryptopp.h"

extern zend_class_entry *cryptopp_ce_MacAbstract;
void init_class_MacAbstract(TSRMLS_D);
void init_class_MacAbstractChild(const char *algoName, const char* className, zend_class_entry **classEntryPtr, zend_function_entry *classMethods TSRMLS_DC);

CryptoPP::MessageAuthenticationCode *getCryptoppMacNativePtr(zval *this_ptr TSRMLS_DC);
void setCryptoppMacNativePtr(zval *this_ptr, CryptoPP::MessageAuthenticationCode *nativePtr TSRMLS_DC);

/* {{{ get the pointer to the native mac object of a php hash class */
#define CRYPTOPP_MAC_ABSTRACT_GET_NATIVE_PTR(ptrName)           \
    ptrName = getCryptoppMacNativePtr(getThis() TSRMLS_CC);     \
                                                                \
    if (NULL == ptrName) {                                      \
        RETURN_FALSE;                                           \
    }                                                           \
/* }}} */

/* {{{ object creation related stuff */
struct MacAbstractContainer {
    zend_object std;
    CryptoPP::MessageAuthenticationCode *mac;
};

extern zend_object_handlers MacAbstract_object_handlers;
void MacAbstract_free_storage(void *object TSRMLS_DC);
zend_object_value MacAbstract_create_handler(zend_class_entry *type TSRMLS_DC);
/* }}} */

/* {{{ methods declarations */
PHP_METHOD(Cryptopp_MacAbstract, __sleep);
PHP_METHOD(Cryptopp_MacAbstract, __wakeup);
PHP_METHOD(Cryptopp_MacAbstract, getName);
PHP_METHOD(Cryptopp_MacAbstract, getDigestSize);
PHP_METHOD(Cryptopp_MacAbstract, getBlockSize);
PHP_METHOD(Cryptopp_MacAbstract, isValidKeyLength);
PHP_METHOD(Cryptopp_MacAbstract, setKey);
PHP_METHOD(Cryptopp_MacAbstract, getKey);
PHP_METHOD(Cryptopp_MacAbstract, calculateDigest);
PHP_METHOD(Cryptopp_MacAbstract, update);
PHP_METHOD(Cryptopp_MacAbstract, finalize);
PHP_METHOD(Cryptopp_MacAbstract, restart);
/* }}} */

/* {{{ php hash classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_MacAbstract___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_MacAbstract___sleep, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* verify that a key size is valid for a MacAbstract instance */
bool isCryptoppMacKeyValid(zval *object, CryptoPP::MessageAuthenticationCode *mac TSRMLS_DC);

#endif /* PHP_MAC_ABSTRACT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
