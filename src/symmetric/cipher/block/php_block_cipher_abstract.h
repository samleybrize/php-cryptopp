/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_BLOCK_CIPHER_ABSTRACT_H
#define PHP_BLOCK_CIPHER_ABSTRACT_H

#include "src/php_cryptopp.h"

extern zend_class_entry *cryptopp_ce_BlockCipherAbstract;
void init_class_BlockCipherAbstract(TSRMLS_D);
void init_class_BlockCipherAbstractChild(const char *algoName, const char* className, zend_class_entry **classEntryPtr, zend_function_entry *classMethods TSRMLS_DC);

CryptoPP::BlockCipher *getCryptoppBlockCipherEncryptorPtr(zval *this_ptr TSRMLS_DC);
CryptoPP::BlockCipher *getCryptoppBlockCipherDecryptorPtr(zval *this_ptr TSRMLS_DC);
void setCryptoppBlockCipherEncryptorPtr(zval *this_ptr, CryptoPP::BlockCipher *nativePtr TSRMLS_DC);
void setCryptoppBlockCipherDecryptorPtr(zval *this_ptr, CryptoPP::BlockCipher *nativePtr TSRMLS_DC);
bool isCryptoppBlockCipherKeyValid(zval *object, CryptoPP::BlockCipher *cipher TSRMLS_DC, bool throwIfFalse = true);

/* {{{ get the pointer to the native encryptor object of a php cipher class */
#define CRYPTOPP_BLOCK_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(ptrName)       \
    ptrName = getCryptoppBlockCipherEncryptorPtr(getThis() TSRMLS_CC);  \
                                                                        \
    if (NULL == ptrName) {                                              \
        RETURN_FALSE;                                                   \
    }                                                                   \
/* }}} */

/* {{{ get the pointer to the native decryptor object of a php cipher class */
#define CRYPTOPP_BLOCK_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(ptrName)       \
    ptrName = getCryptoppBlockCipherDecryptorPtr(getThis() TSRMLS_CC);  \
                                                                        \
    if (NULL == ptrName) {                                              \
        RETURN_FALSE;                                                   \
    }                                                                   \
/* }}} */

/* {{{ object creation related stuff */
struct BlockCipherAbstractContainer {
    zend_object std;
    CryptoPP::BlockCipher *encryptor;
    CryptoPP::BlockCipher *decryptor;
};

extern zend_object_handlers BlockCipherAbstract_object_handlers;
void BlockCipherAbstract_free_storage(void *object TSRMLS_DC);
/* }}} */

/* {{{ methods declarations */
PHP_METHOD(Cryptopp_BlockCipherAbstract, __sleep);
PHP_METHOD(Cryptopp_BlockCipherAbstract, __wakeup);
PHP_METHOD(Cryptopp_BlockCipherAbstract, getName);
PHP_METHOD(Cryptopp_BlockCipherAbstract, getBlockSize);
PHP_METHOD(Cryptopp_BlockCipherAbstract, isValidKeyLength);
PHP_METHOD(Cryptopp_BlockCipherAbstract, setKey);
PHP_METHOD(Cryptopp_BlockCipherAbstract, getKey);
PHP_METHOD(Cryptopp_BlockCipherAbstract, encryptBlock);
PHP_METHOD(Cryptopp_BlockCipherAbstract, decryptBlock);
PHP_METHOD(Cryptopp_BlockCipherAbstract, encrypt);
PHP_METHOD(Cryptopp_BlockCipherAbstract, decrypt);
/* }}} */

/* {{{ php cipher classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_BlockCipherAbstract___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_BlockCipherAbstract___sleep, 0)
ZEND_END_ARG_INFO()
/* }}} */

#endif /* PHP_BLOCK_CIPHER_ABSTRACT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
