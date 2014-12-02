#ifndef PHP_SYMMETRIC_CIPHER_ABSTRACT_H
#define PHP_SYMMETRIC_CIPHER_ABSTRACT_H

#include "../../php_cryptopp.h"

extern zend_class_entry *cryptopp_ce_SymmetricCipherAbstract;
void init_class_SymmetricCipherAbstract(TSRMLS_D);
void init_class_SymmetricCipherAbstractChild(const char *algoName, const char* className, zend_class_entry *classEntryPtr, zend_function_entry *classMethods TSRMLS_DC);

CryptoPP::BlockCipher *getCryptoppSymmetricCipherEncryptorPtr(zval *this_ptr TSRMLS_DC);
CryptoPP::BlockCipher *getCryptoppSymmetricCipherDecryptorPtr(zval *this_ptr TSRMLS_DC);
void setCryptoppSymmetricCipherEncryptorPtr(zval *this_ptr, CryptoPP::BlockCipher *nativePtr TSRMLS_DC);
void setCryptoppSymmetricCipherDecryptorPtr(zval *this_ptr, CryptoPP::BlockCipher *nativePtr TSRMLS_DC);

/* {{{ get the pointer to the native encryptor object of a php cipher class */
#define CRYPTOPP_SYMMETRIC_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(ptrName)       \
    ptrName = getCryptoppSymmetricCipherEncryptorPtr(getThis() TSRMLS_CC);  \
                                                                            \
    if (NULL == ptrName) {                                                  \
        RETURN_FALSE;                                                       \
    }                                                                       \
/* }}} */

/* {{{ get the pointer to the native decryptor object of a php cipher class */
#define CRYPTOPP_SYMMETRIC_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(ptrName)       \
    ptrName = getCryptoppSymmetricCipherDecryptorPtr(getThis() TSRMLS_CC);  \
                                                                            \
    if (NULL == ptrName) {                                                  \
        RETURN_FALSE;                                                       \
    }                                                                       \
/* }}} */

/* {{{ object creation related stuff */
struct SymmetricCipherAbstractContainer {
    zend_object std;
    CryptoPP::BlockCipher *encryptor;
    CryptoPP::BlockCipher *decryptor;
};

extern zend_object_handlers SymmetricCipherAbstract_object_handlers;
void SymmetricCipherAbstract_free_storage(void *object TSRMLS_DC);
zend_object_value SymmetricCipherAbstract_create_handler(zend_class_entry *type TSRMLS_DC);
/* }}} */

/* {{{ methods declarations */
PHP_METHOD(Cryptopp_SymmetricCipherAbstract, __sleep);
PHP_METHOD(Cryptopp_SymmetricCipherAbstract, __wakeup);
PHP_METHOD(Cryptopp_SymmetricCipherAbstract, getName);
/* }}} */

/* {{{ php cipher classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_SymmetricCipherAbstract___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricCipherAbstract___sleep, 0)
ZEND_END_ARG_INFO()
/* }}} */

#endif /* PHP_SYMMETRIC_CIPHER_ABSTRACT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
