/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_HASH_ABSTRACT_H
#define PHP_HASH_ABSTRACT_H

#include "src/php_cryptopp.h"

extern zend_class_entry *cryptopp_ce_HashAbstract;
void init_class_HashAbstract(TSRMLS_D);
void init_class_HashAbstractChild(const char *algoName, const char* className, zend_class_entry **classEntryPtr, zend_function_entry *classMethods TSRMLS_DC);

CryptoPP::HashTransformation *getCryptoppHashNativePtr(zval *this_ptr TSRMLS_DC);
void setCryptoppHashNativePtr(zval *this_ptr, CryptoPP::HashTransformation *nativePtr TSRMLS_DC);

/* {{{ get the pointer to the native hash object of a php hash class */
#define CRYPTOPP_HASH_ABSTRACT_GET_NATIVE_PTR(ptrName)          \
    ptrName = getCryptoppHashNativePtr(getThis() TSRMLS_CC);    \
                                                                \
    if (NULL == ptrName) {                                      \
        RETURN_FALSE;                                           \
    }                                                           \
/* }}} */

/* {{{ object creation related stuff */
struct HashAbstractContainer {
    zend_object std;
    CryptoPP::HashTransformation *hash;
};

extern zend_object_handlers HashAbstract_object_handlers;
void HashAbstract_free_storage(void *object TSRMLS_DC);
/* }}} */

/* {{{ methods declarations */
PHP_METHOD(Cryptopp_HashAbstract, __sleep);
PHP_METHOD(Cryptopp_HashAbstract, __wakeup);
PHP_METHOD(Cryptopp_HashAbstract, getName);
PHP_METHOD(Cryptopp_HashAbstract, getDigestSize);
PHP_METHOD(Cryptopp_HashAbstract, getBlockSize);
PHP_METHOD(Cryptopp_HashAbstract, calculateDigest);
PHP_METHOD(Cryptopp_HashAbstract, update);
PHP_METHOD(Cryptopp_HashAbstract, finalize);
PHP_METHOD(Cryptopp_HashAbstract, restart);
/* }}} */

/* {{{ php hash classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_HashAbstract___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashAbstract___sleep, 0)
ZEND_END_ARG_INFO()
/* }}} */

#endif /* PHP_HASH_ABSTRACT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
