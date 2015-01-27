/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "src/php_cryptopp.h"
#include "php_hash_abstract.h"
#include "php_md5.h"
#include <md5.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_HashMd5___construct, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_HashMd5;

static zend_function_entry cryptopp_methods_HashMd5[] = {
    PHP_ME(Cryptopp_HashMd5, __construct, arginfo_HashMd5___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

void init_class_HashMd5(TSRMLS_D) {
    init_class_HashAbstractChild("md5", "HashMd5", &cryptopp_ce_HashMd5, cryptopp_methods_HashMd5 TSRMLS_CC);
}
/* }}} */

/* {{{ proto HashMd5::__construct(void) */
PHP_METHOD(Cryptopp_HashMd5, __construct) {
    CryptoPP::Weak::MD5 *hash = new CryptoPP::Weak::MD5();
    setCryptoppHashNativePtr(getThis(), hash TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_HashAbstract, getThis(), "name", 4, "md5", 3 TSRMLS_CC);
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
