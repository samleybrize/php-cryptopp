/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_STREAM_CIPHER_INTERFACE_H
#define PHP_STREAM_CIPHER_INTERFACE_H

#include "src/php_cryptopp.h"

extern zend_class_entry *cryptopp_ce_StreamCipherInterface;
void init_interface_StreamCipherInterface(TSRMLS_D);

#endif /* PHP_STREAM_CIPHER_INTERFACE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
