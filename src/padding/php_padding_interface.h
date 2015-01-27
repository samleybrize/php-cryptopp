/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_PADDING_INTERFACE_H
#define PHP_PADDING_INTERFACE_H

#include "src/php_cryptopp.h"

extern zend_class_entry *cryptopp_ce_PaddingInterface;
void init_interface_PaddingInterface(TSRMLS_D);

/* {{{ php padding classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_PaddingInterface_pad, 0)
    ZEND_ARG_INFO(0, data)
    ZEND_ARG_INFO(0, blockSize)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_PaddingInterface_unpad, 0)
    ZEND_ARG_INFO(0, data)
    ZEND_ARG_INFO(0, blockSize)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_PaddingInterface_canPad, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_PaddingInterface_canUnpad, 0)
ZEND_END_ARG_INFO()
/* }}} */

#endif /* PHP_PADDING_INTERFACE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
