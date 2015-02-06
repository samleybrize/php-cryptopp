/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_MAC_TTMAC_H
#define PHP_MAC_TTMAC_H

#include "src/php_cryptopp.h"

void init_class_MacTwoTrackMac(TSRMLS_D);
PHP_METHOD(Cryptopp_MacTwoTrackMac, __construct);

#endif /* PHP_MAC_TTMAC_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
