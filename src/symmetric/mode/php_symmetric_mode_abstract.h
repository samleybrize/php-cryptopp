#ifndef PHP_SYMMETRIC_MODE_ABSTRACT_H
#define PHP_SYMMETRIC_MODE_ABSTRACT_H

#include "../../php_cryptopp.h"
#include <modes.h>

extern zend_class_entry *cryptopp_ce_SymmetricModeAbstract;
void init_class_SymmetricModeAbstract(TSRMLS_D);
void init_class_SymmetricModeAbstractChild(const char *modeName, const char* className, zend_class_entry *classEntryPtr, zend_function_entry *classMethods TSRMLS_DC);

CryptoPP::CipherModeBase *getCryptoppSymmetricModeEncryptorPtr(zval *this_ptr TSRMLS_DC);
CryptoPP::CipherModeBase *getCryptoppSymmetricModeDecryptorPtr(zval *this_ptr TSRMLS_DC);
void setCryptoppSymmetricModeEncryptorPtr(zval *this_ptr, CryptoPP::CipherModeBase *nativePtr TSRMLS_DC);
void setCryptoppSymmetricModeDecryptorPtr(zval *this_ptr, CryptoPP::CipherModeBase *nativePtr TSRMLS_DC);

/* {{{ get the pointer to the native encryptor object of a php mode class */
#define CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_ENCRYPTOR_PTR(ptrName)         \
    ptrName = getCryptoppSymmetricModeEncryptorPtr(getThis() TSRMLS_CC);    \
                                                                            \
    if (NULL == ptrName) {                                                  \
        RETURN_FALSE;                                                       \
    }                                                                       \
/* }}} */

/* {{{ get the pointer to the native decryptor object of a php mode class */
#define CRYPTOPP_SYMMETRIC_MODE_ABSTRACT_GET_DECRYPTOR_PTR(ptrName)         \
    ptrName = getCryptoppSymmetricModeDecryptorPtr(getThis() TSRMLS_CC);    \
                                                                            \
    if (NULL == ptrName) {                                                  \
        RETURN_FALSE;                                                       \
    }                                                                       \
/* }}} */

/* {{{ object creation related stuff */
struct SymmetricModeAbstractContainer {
    zend_object std;
    CryptoPP::CipherModeBase *encryptor;
    CryptoPP::CipherModeBase *decryptor;
};

extern zend_object_handlers SymmetricModeAbstract_object_handlers;
void SymmetricModeAbstract_free_storage(void *object TSRMLS_DC);
zend_object_value SymmetricModeAbstract_create_handler(zend_class_entry *type TSRMLS_DC);
/* }}} */

/* {{{ methods declarations */
PHP_METHOD(Cryptopp_SymmetricModeAbstract, __sleep);
PHP_METHOD(Cryptopp_SymmetricModeAbstract, __wakeup);
PHP_METHOD(Cryptopp_SymmetricModeAbstract, getName);
PHP_METHOD(Cryptopp_SymmetricModeAbstract, setKey);
/* }}} */

/* {{{ php mode classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_SymmetricModeAbstract___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricModeAbstract___sleep, 0)
ZEND_END_ARG_INFO()
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

#endif /* PHP_SYMMETRIC_MODE_ABSTRACT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
