/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "src/php_cryptopp.h"
#include "php_rbg_interface.h"

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_RandomByteGeneratorInterface;

static zend_function_entry cryptopp_methods_RandomByteGeneratorInterface[] = {
    PHP_ABSTRACT_ME(Cryptopp_RandomByteGeneratorInterface, generate, arginfo_RandomByteGeneratorInterface_generate)
    PHP_FE_END
};

void init_interface_RandomByteGeneratorInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "RandomByteGeneratorInterface", cryptopp_methods_RandomByteGeneratorInterface);
    cryptopp_ce_RandomByteGeneratorInterface = zend_register_internal_interface(&ce TSRMLS_CC);
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
