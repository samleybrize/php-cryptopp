#include "../php_cryptopp.h"
#include "php_hash_interface.h"
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
    CRYPTOPP_HASH_REQUIRED_METHODS(HashSha1)
    PHP_FE_END
};

CRYPTOPP_HASH_INIT_CLASS("sha1", HashSha1, cryptopp_ce_HashSha1, cryptopp_methods_HashSha1)
/* }}} */

/* {{{ proto HashSha1::__construct(void) */
PHP_METHOD(Cryptopp_HashSha1, __construct) {
    CryptoPP::SHA1 *hash;
    hash = new CryptoPP::SHA1();
    CRYPTOPP_HASH_SET_NATIVE_PTR(hash)

    zend_update_property_stringl(cryptopp_ce_HashSha1, getThis(), "name", 4, "sha1", 4 TSRMLS_CC);
}
/* }}} */

/* {{{ proto string HashSha1::getName(void)
   Return algorithm name */
PHP_METHOD(Cryptopp_HashSha1, getName) {
    RETURN_STRING("sha1", 1);
}
/* }}} */

/* include common hash methods definitions */
CRYPTOPP_HASH_COMMON_METHODS_DEFINITIONS(HashSha1, CryptoPP::SHA1)

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
