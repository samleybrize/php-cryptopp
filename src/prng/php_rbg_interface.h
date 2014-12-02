#ifndef PHP_RBG_INTERFACE_H
#define PHP_RBG_INTERFACE_H

#include "../php_cryptopp.h"

extern zend_class_entry *cryptopp_ce_RandomByteGeneratorInterface;
void init_interface_RandomByteGeneratorInterface(TSRMLS_D);

/* {{{ php rbg classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_RandomByteGeneratorInterface___sleep, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_RandomByteGeneratorInterface___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_RandomByteGeneratorInterface_generate, 0)
    ZEND_ARG_INFO(0, size)
ZEND_END_ARG_INFO()
/* }}} */

#endif /* PHP_RBG_INTERFACE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
