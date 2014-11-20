#include "../php_cryptopp.h"
#include "php_hash_interface.h"
#include "php_md5.h"
#include <md5.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_HashMd5_construct, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_HashMd5;

static zend_function_entry cryptopp_methods_HashMd5[] = {
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_HashMd5, __construct, arginfo_HashMd5_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    CRYPTOPP_HASH_REQUIRED_METHODS(HashMd5)
    PHP_FE_END
};

CRYPTOPP_HASH_INIT_CLASS("md5", HashMd5, cryptopp_ce_HashMd5, cryptopp_methods_HashMd5)
/* }}} */

/* {{{ proto HashMd5::__construct(void) */
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashMd5, __construct) {
    CryptoPP::Weak::MD5 *hash;
    hash = new CryptoPP::Weak::MD5();
    CRYPTOPP_HASH_SET_NATIVE_PTR(hash)
}
/* }}} */

/* {{{ proto string HashMd5::getName(void)
   Return algorithm name */
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashMd5, getName) {
    RETURN_STRING("md5", 1);
}
/* }}} */

/* include common hash methods definitions */
CRYPTOPP_HASH_COMMON_METHODS_DEFINITIONS(HashMd5, CryptoPP::Weak::MD5)

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
