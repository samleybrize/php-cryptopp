/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_HASH_TRANSFORMATION_INTERFACE_H
#define PHP_HASH_TRANSFORMATION_INTERFACE_H

#include "src/php_cryptopp.h"

/* {{{ php hash classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_HashTransformationInterface_getName, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashTransformationInterface_getDigestSize, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashTransformationInterface_getBlockSize, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashTransformationInterface_calculateDigest, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashTransformationInterface_update, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashTransformationInterface_finalize, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashTransformationInterface_restart, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* inits the HashTransformationInterface php interface */
extern zend_class_entry *cryptopp_ce_HashTransformationInterface;
void init_interface_HashTransformationInterface(TSRMLS_D);

#endif /* PHP_HASH_TRANSFORMATION_INTERFACE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
