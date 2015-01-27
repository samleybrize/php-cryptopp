/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_MAC_INTERFACE_H
#define PHP_MAC_INTERFACE_H

#include "src/php_cryptopp.h"

extern zend_class_entry *cryptopp_ce_MacInterface;
void init_interface_MacInterface(TSRMLS_D);

/* {{{ php Mac classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_MacInterface_setKey, 0)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_MacInterface_getKey, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_MacInterface_isValidKeyLength, 0)
    ZEND_ARG_INFO(0, keyLength)
ZEND_END_ARG_INFO()
/* }}} */

#endif /* PHP_MAC_INTERFACE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
