/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_CRYPTOPP_INIT_H
#define PHP_CRYPTOPP_INIT_H

#include "php_cryptopp.h"

//%configure_inclusion%

const zend_function_entry php_cryptopp_functions[] = {
//%configure_functions%
    PHP_FE_END
};

#endif /* PHP_CRYPTOPP_INIT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
