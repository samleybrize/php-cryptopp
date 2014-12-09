#ifndef PHP_SYMMETRIC_TRANSFORMATION_INTERFACE_H
#define PHP_SYMMETRIC_TRANSFORMATION_INTERFACE_H

#include "../../php_cryptopp.h"

/* {{{ php symmetric transformation classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_SymmetricTransformationInterface_setKey, 0)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricTransformationInterface_setIv, 0)
    ZEND_ARG_INFO(0, iv)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricTransformationInterface_getBlockSize, 0)
ZEND_END_ARG_INFO()

// TODO optimalBlockSize??

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricTransformationInterface_encryptData, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricTransformationInterface_decryptData, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricTransformationInterface_restart, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* inits the SymmetricTransformationInterface php interface */
extern zend_class_entry *cryptopp_ce_SymmetricTransformationInterface;
void init_interface_SymmetricTransformationInterface(TSRMLS_D);

#endif /* PHP_SYMMETRIC_TRANSFORMATION_INTERFACE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
