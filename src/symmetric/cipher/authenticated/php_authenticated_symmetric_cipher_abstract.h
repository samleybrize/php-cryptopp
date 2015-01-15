#ifndef PHP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_H
#define PHP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_H

#include "src/php_cryptopp.h"

extern zend_class_entry *cryptopp_ce_AuthenticatedSymmetricCipherAbstract;
void init_class_AuthenticatedSymmetricCipherAbstract(TSRMLS_D);
void init_class_AuthenticatedSymmetricCipherAbstractChild(const char *algoName, const char* className, zend_class_entry **classEntryPtr, zend_function_entry *classMethods TSRMLS_DC);

CryptoPP::AuthenticatedSymmetricCipher *getCryptoppAuthenticatedSymmetricCipherEncryptorPtr(zval *this_ptr TSRMLS_DC);
CryptoPP::AuthenticatedSymmetricCipher *getCryptoppAuthenticatedSymmetricCipherDecryptorPtr(zval *this_ptr TSRMLS_DC);
void setCryptoppAuthenticatedSymmetricCipherEncryptorPtr(zval *this_ptr, CryptoPP::AuthenticatedSymmetricCipher *nativePtr TSRMLS_DC);
void setCryptoppAuthenticatedSymmetricCipherDecryptorPtr(zval *this_ptr, CryptoPP::AuthenticatedSymmetricCipher *nativePtr TSRMLS_DC);

/* {{{ get the pointer to the native encryptor object of a php cipher class */
#define CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(ptrName)     \
    ptrName = getCryptoppAuthenticatedSymmetricCipherEncryptorPtr(getThis() TSRMLS_CC); \
                                                                        \
    if (NULL == ptrName) {                                              \
        RETURN_FALSE;                                                   \
    }                                                                   \
/* }}} */

/* {{{ get the pointer to the native decryptor object of a php cipher class */
#define CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(ptrName)     \
    ptrName = getCryptoppAuthenticatedSymmetricCipherDecryptorPtr(getThis() TSRMLS_CC); \
                                                                        \
    if (NULL == ptrName) {                                              \
        RETURN_FALSE;                                                   \
    }                                                                   \
/* }}} */

/* {{{ object creation related stuff */
struct AuthenticatedSymmetricCipherAbstractContainer {
    zend_object std;
    CryptoPP::AuthenticatedSymmetricCipher *encryptor;
    CryptoPP::AuthenticatedSymmetricCipher *decryptor;
};

extern zend_object_handlers AuthenticatedSymmetricCipherAbstract_object_handlers;
void AuthenticatedSymmetricCipherAbstract_free_storage(void *object TSRMLS_DC);
/* }}} */

/* {{{ methods declarations */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, __sleep);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, __wakeup);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, getName);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, getBlockSize);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, getDigestSize);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, isValidKeyLength);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, isValidIvLength);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, setKey);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, setIv);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, getKey);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, getIv);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, encrypt);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, decrypt);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, addEncryptionAdditionalData);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, addDecryptionAdditionalData);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, finalizeEncryption);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, finalizeDecryption);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, restart);
/* }}} */

/* {{{ php cipher classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherAbstract___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherAbstract___sleep, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ Get needed cipher elements to build an authenticated cipher object */
bool cryptoppAuthenticatedSymmetricCipherGetCipherElements(
    const char *authenticatedCipherName,
    zval *cipherObject,
    zval *authenticatedCipherObject,
    CryptoPP::BlockCipher **cipherEncryptor,
    CryptoPP::BlockCipher **cipherDecryptor,
    std::string **authenticatedCipherFullName,
    bool &cipherMustBeDestructed
    TSRMLS_DC
);
/* }}} */

/* verify that a key size is valid for an AuthenticatedSymmetricCipherAbstract instance */
bool isCryptoppAuthenticatedSymmetricCipherKeyValid(zval *object, CryptoPP::AuthenticatedSymmetricCipher *cipher TSRMLS_DC);

/* verify that an iv size is valid for an AuthenticatedSymmetricCipherAbstract instance */
bool isCryptoppAuthenticatedSymmetricCipherIvValid(zval *object, CryptoPP::AuthenticatedSymmetricCipher *cipher TSRMLS_DC);

#endif /* PHP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
