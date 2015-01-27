/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_BLOCK_CIPHER_INTERFACE_H
#define PHP_BLOCK_CIPHER_INTERFACE_H

#include "src/php_cryptopp.h"

extern zend_class_entry *cryptopp_ce_BlockCipherInterface;
void init_interface_BlockCipherInterface(TSRMLS_D);

/* {{{ php block cipher classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_BlockCipherInterface_encryptBlock, 0)
    ZEND_ARG_INFO(0, block)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_BlockCipherInterface_decryptBlock, 0)
    ZEND_ARG_INFO(0, block)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_BlockCipherInterface_encrypt, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_BlockCipherInterface_decrypt, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()
/* }}} */

#endif /* PHP_BLOCK_CIPHER_INTERFACE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
