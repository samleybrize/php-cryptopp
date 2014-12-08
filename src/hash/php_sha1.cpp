#include "../php_cryptopp.h"
#include "php_hash_abstract.h"
#include "php_sha1.h"
#include <sha.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_HashSha1_construct, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_HashSha1;

static zend_function_entry cryptopp_methods_HashSha1[] = {
    PHP_ME(Cryptopp_HashSha1, __construct, arginfo_HashSha1_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

void init_class_HashSha1(TSRMLS_D) {
    init_class_HashAbstractChild("sha1", "HashSha1", cryptopp_ce_HashSha1, cryptopp_methods_HashSha1 TSRMLS_CC);
}
/* }}} */

/* {{{ proto HashSha1::__construct(void) */
PHP_METHOD(Cryptopp_HashSha1, __construct) {
    CryptoPP::SHA1 *hash;
    hash = new CryptoPP::SHA1();
    setCryptoppHashNativePtr(getThis(), hash TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_HashAbstract, getThis(), "name", 4, "sha1", 4 TSRMLS_CC);
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
