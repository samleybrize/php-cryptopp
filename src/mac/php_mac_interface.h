#ifndef PHP_MAC_INTERFACE_H
#define PHP_MAC_INTERFACE_H

#include "../php_cryptopp.h"

/* {{{ php Mac classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_MacInterface_setKey, 0)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_MacInterface_isValidKeyLength, 0)
    ZEND_ARG_INFO(0, keyLength)
ZEND_END_ARG_INFO()
/* }}} */

/* inits the MacInterface php interface */
extern zend_class_entry *cryptopp_ce_MacInterface;
void init_interface_MacInterface(TSRMLS_D);

#endif /* PHP_MAC_INTERFACE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
