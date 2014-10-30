#include "../php_cryptopp.h"
#include "php_hash_interface.h"
#include "php_sha3.h"
#include <sha3.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_HashSha3_256_construct, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP classes déclaration */
zend_class_entry *cryptopp_ce_HashSha3_256;

static zend_function_entry cryptopp_methods_HashSha3_256[] = {
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_HashSha3_256, __construct, arginfo_HashSha3_256_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    CRYPTOPP_HASH_REQUIRED_METHODS(HashSha3_256)
    PHP_FE_END
};

CRYPTOPP_HASH_INIT_CLASS("sha3_256", HashSha3_256, CryptoPP::SHA3_256, cryptopp_ce_HashSha3_256, cryptopp_methods_HashSha3_256)

void init_classes_HashSha3(TSRMLS_D)
{
    CRYPTOPP_HASH_INIT_CLASS_FUNC_CALL(HashSha3_256)
}
/* }}} */

/* {{{ HashSha3_256 constructor */
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashSha3_256, __construct) {
    CryptoPP::SHA3_256 *hash;
    hash = new CryptoPP::SHA3_256();
    CRYPTOPP_HASH_SET_NATIVE_PTR(HashSha3_256, hash)
}
/* }}} */

/* {{{ proto string getName()
   Return algorithm name */
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashSha3_256, getName) {
    RETURN_STRING("sha3_256", 1);
}
/* }}} */

/* include common hash methods definitions */
CRYPTOPP_HASH_COMMON_METHODS_DEFINITIONS(HashSha3_256, CryptoPP::SHA3_256)
