#ifndef PHP_SYMMETRIC_MODE_INTERFACE_H
#define PHP_SYMMETRIC_MODE_INTERFACE_H

#include "../../php_cryptopp.h"

/* {{{ php symmetric mode classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_SymmetricModeInterface_getName, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricModeInterface_setKey, 0)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricModeInterface_setIv, 0)
    ZEND_ARG_INFO(0, iv)
ZEND_END_ARG_INFO()
/* }}} */

/* inits the SymmetricModeInterface php interface */
extern zend_class_entry *cryptopp_ce_SymmetricModeInterface;
void init_interface_SymmetricModeInterface(TSRMLS_D);

#endif /* PHP_SYMMETRIC_MODE_INTERFACE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
