#ifndef PHP_HASH_INTERFACE_H
#define PHP_HASH_INTERFACE_H

#include "../php_cryptopp.h"

/* {{{ php hash classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_getName, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_getDigestSize, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_getBlockSize, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_calculateDigest, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_update, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_final, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_restart, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* inits the HashInterface php interface */
extern zend_class_entry *cryptopp_ce_HashInterface;
void init_interface_HashInterface(TSRMLS_D);

#endif /* PHP_HASH_INTERFACE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
