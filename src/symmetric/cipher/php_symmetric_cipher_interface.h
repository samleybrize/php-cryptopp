#ifndef PHP_SYMMETRIC_CIPHER_INTERFACE_H
#define PHP_SYMMETRIC_CIPHER_INTERFACE_H

#include "../../php_cryptopp.h"

/* {{{ php symmetric cipher classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_SymmetricCipherInterface_getName, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* inits the SymmetricCipherInterface php interface */
extern zend_class_entry *cryptopp_ce_SymmetricCipherInterface;
void init_interface_SymmetricCipherInterface(TSRMLS_D);

#endif /* PHP_SYMMETRIC_CIPHER_INTERFACE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
