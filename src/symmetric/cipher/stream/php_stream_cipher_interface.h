#ifndef PHP_STREAM_CIPHER_INTERFACE_H
#define PHP_STREAM_CIPHER_INTERFACE_H

#include "../../../php_cryptopp.h"

/* {{{ php stream cipher classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_StreamCipherInterface_setKey, 0)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_StreamCipherInterface_setIv, 0)
    ZEND_ARG_INFO(0, iv)
ZEND_END_ARG_INFO()
/* }}} */

/* inits the StreamCipherInterface php interface */
extern zend_class_entry *cryptopp_ce_StreamCipherInterface;
void init_interface_StreamCipherInterface(TSRMLS_D);

#endif /* PHP_STREAM_CIPHER_INTERFACE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
