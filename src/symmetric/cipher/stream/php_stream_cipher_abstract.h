#ifndef PHP_STREAM_CIPHER_ABSTRACT_H
#define PHP_STREAM_CIPHER_ABSTRACT_H

#include "../../../php_cryptopp.h"

extern zend_class_entry *cryptopp_ce_StreamCipherAbstract;
void init_class_StreamCipherAbstract(TSRMLS_D);
void init_class_StreamCipherAbstractChild(const char *algoName, const char* className, zend_class_entry **classEntryPtr, zend_function_entry *classMethods TSRMLS_DC);

CryptoPP::SymmetricCipher *getCryptoppStreamCipherEncryptorPtr(zval *this_ptr TSRMLS_DC);
CryptoPP::SymmetricCipher *getCryptoppStreamCipherDecryptorPtr(zval *this_ptr TSRMLS_DC);
void setCryptoppStreamCipherEncryptorPtr(zval *this_ptr, CryptoPP::SymmetricCipher *nativePtr TSRMLS_DC);
void setCryptoppStreamCipherDecryptorPtr(zval *this_ptr, CryptoPP::SymmetricCipher *nativePtr TSRMLS_DC);

/* {{{ get the pointer to the native encryptor object of a php cipher class */
#define CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(ptrName)       \
    ptrName = getCryptoppStreamCipherEncryptorPtr(getThis() TSRMLS_CC);  \
                                                                        \
    if (NULL == ptrName) {                                              \
        RETURN_FALSE;                                                   \
    }                                                                   \
/* }}} */

/* {{{ get the pointer to the native decryptor object of a php cipher class */
#define CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(ptrName)       \
    ptrName = getCryptoppStreamCipherDecryptorPtr(getThis() TSRMLS_CC);  \
                                                                        \
    if (NULL == ptrName) {                                              \
        RETURN_FALSE;                                                   \
    }                                                                   \
/* }}} */

/* {{{ object creation related stuff */
struct StreamCipherAbstractContainer {
    zend_object std;
    CryptoPP::SymmetricCipher *encryptor;
    CryptoPP::SymmetricCipher *decryptor;
};

extern zend_object_handlers StreamCipherAbstract_object_handlers;
void StreamCipherAbstract_free_storage(void *object TSRMLS_DC);
zend_object_value StreamCipherAbstract_create_handler(zend_class_entry *type TSRMLS_DC);
/* }}} */

/* {{{ methods declarations */
PHP_METHOD(Cryptopp_StreamCipherAbstract, __sleep);
PHP_METHOD(Cryptopp_StreamCipherAbstract, __wakeup);
PHP_METHOD(Cryptopp_StreamCipherAbstract, getName);
PHP_METHOD(Cryptopp_StreamCipherAbstract, setKey);
PHP_METHOD(Cryptopp_StreamCipherAbstract, setIv);
PHP_METHOD(Cryptopp_StreamCipherAbstract, getBlockSize);
PHP_METHOD(Cryptopp_StreamCipherAbstract, encrypt);
PHP_METHOD(Cryptopp_StreamCipherAbstract, decrypt);
PHP_METHOD(Cryptopp_StreamCipherAbstract, restart);
/* }}} */

/* {{{ php cipher classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_StreamCipherAbstract___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_StreamCipherAbstract___sleep, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* verify that a key size is valid for a StreamCipherAbstract instance */
bool isCryptoppStreamCipherKeyValid(zval *object, CryptoPP::SymmetricCipher *mode);

/* verify that an iv size is valid for a StreamCipherAbstract instance */
bool isCryptoppStreamCipherIvValid(zval *object, CryptoPP::SymmetricCipher *mode);

#endif /* PHP_STREAM_CIPHER_ABSTRACT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
