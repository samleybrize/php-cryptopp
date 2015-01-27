/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_HASH_TRANSFORMATION_FILTER_H
#define PHP_HASH_TRANSFORMATION_FILTER_H

#include "src/php_cryptopp.h"
#include <filters.h>
#include <string>

void init_class_HashTransformationFilter(TSRMLS_D);

/* {{{ adds hash destruction support to CryptoPP::HashFilter */
class HashTransformationFilter : public CryptoPP::HashFilter
{
public:
    HashTransformationFilter(CryptoPP::HashTransformation &hash, bool hashMustBeDestructed);
    ~HashTransformationFilter();

protected:
    CryptoPP::HashTransformation &m_hash;
    bool m_hashMustBeDestructed;
};
/* }}} */

/* {{{ CRYPTOPP_HASH_TRANSFORMATION_FILTER_GET_NATIVE_PTR
   get the pointer to the native htf encryptor object of the php class */
#define CRYPTOPP_HASH_TRANSFORMATION_FILTER_GET_NATIVE_PTR(ptrName)         \
    ptrName = getCryptoppHashTransformationFilterNativePtr(getThis() TSRMLS_CC); \
                                                                            \
    if (NULL == ptrName) {                                                  \
        RETURN_FALSE;                                                       \
    }                                                                       \
/* }}} */

/* {{{ object creation related stuff */
struct HashTransformationFilterContainer {
    zend_object std;
    HashTransformationFilter *htf;
};

extern zend_object_handlers HashTransformationFilter_object_handlers;
void HashTransformationFilter_free_storage(void *object TSRMLS_DC);
/* }}} */

/* {{{ methods declarations */
PHP_METHOD(Cryptopp_HashTransformationFilter, __construct);
PHP_METHOD(Cryptopp_HashTransformationFilter, __sleep);
PHP_METHOD(Cryptopp_HashTransformationFilter, __wakeup);
PHP_METHOD(Cryptopp_HashTransformationFilter, getHash);
PHP_METHOD(Cryptopp_HashTransformationFilter, calculateDigestString);
/* }}} */

#endif /* PHP_HASH_TRANSFORMATION_FILTER_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
