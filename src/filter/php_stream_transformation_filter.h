#ifndef PHP_STREAM_TRANSFORMATION_FILTER_H
#define PHP_STREAM_TRANSFORMATION_FILTER_H

#include "../php_cryptopp.h"
#include <filters.h>

void init_class_StreamTransformationFilter(TSRMLS_D);

/* {{{ get the pointer to the native stf encryptor object of the php class */
#define CRYPTOPP_STREAM_TRANSFORMATION_FILTER_GET_ENCRYPTOR_PTR(ptrName)    \
    ptrName = getCryptoppStreamTransformationFilterEncryptorPtr(getThis() TSRMLS_CC); \
                                                                            \
    if (NULL == ptrName) {                                                  \
        RETURN_FALSE;                                                       \
    }                                                                       \
/* }}} */

/* {{{ get the pointer to the native stf decryptor object of the php class */
#define CRYPTOPP_STREAM_TRANSFORMATION_FILTER_GET_DECRYPTOR_PTR(ptrName)    \
    ptrName = getCryptoppStreamTransformationFilterDecryptorPtr(getThis() TSRMLS_CC); \
                                                                            \
    if (NULL == ptrName) {                                                  \
        RETURN_FALSE;                                                       \
    }                                                                       \
/* }}} */

/* {{{ object creation related stuff */
struct StreamTransformationFilterContainer {
    zend_object std;
    CryptoPP::StreamTransformationFilter *stfEncryptor;
    CryptoPP::StreamTransformationFilter *stfDecryptor;
};

extern zend_object_handlers StreamTransformationFilter_object_handlers;
void StreamTransformationFilter_free_storage(void *object TSRMLS_DC);
zend_object_value StreamTransformationFilter_create_handler(zend_class_entry *type TSRMLS_DC);
/* }}} */

/* {{{ methods declarations */
PHP_METHOD(Cryptopp_StreamTransformationFilter, __construct);
PHP_METHOD(Cryptopp_StreamTransformationFilter, __sleep);
PHP_METHOD(Cryptopp_StreamTransformationFilter, __wakeup);
PHP_METHOD(Cryptopp_StreamTransformationFilter, getCipherMode);
PHP_METHOD(Cryptopp_StreamTransformationFilter, encryptString);
PHP_METHOD(Cryptopp_StreamTransformationFilter, decryptString);
/* }}} */

#endif /* PHP_STREAM_TRANSFORMATION_FILTER_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
