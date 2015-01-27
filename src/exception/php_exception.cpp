/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "src/php_cryptopp.h"
#include "php_exception.h"
#include <zend_exceptions.h>

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_CryptoppException;

void init_class_CryptoppException(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "CryptoppException", NULL);
    cryptopp_ce_CryptoppException = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
}
/* }}} */

/* {{{ getCryptoppException
   return a pointer to the CryptoppException class entry */
zend_class_entry *getCryptoppException() {
    return cryptopp_ce_CryptoppException;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
