/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "src/php_cryptopp.h"
#include "php_hash_transformation_interface.h"
#include "php_hash_interface.h"

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_HashInterface;

static zend_function_entry cryptopp_methods_HashInterface[] = {
    PHP_FE_END
};

void init_interface_HashInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "HashInterface", cryptopp_methods_HashInterface);
    cryptopp_ce_HashInterface = zend_register_internal_interface(&ce TSRMLS_CC);

    zend_class_implements(cryptopp_ce_HashInterface TSRMLS_CC, 1, cryptopp_ce_HashTransformationInterface);
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
