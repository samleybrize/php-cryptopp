#include "../php_cryptopp.h"
#include "php_hash_interface.h"
#include "php_sha3.h"
#include <sha3.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_HashSha3_224_construct, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashSha3_256_construct, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashSha3_384_construct, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashSha3_512_construct, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP classes déclaration */
zend_class_entry *cryptopp_ce_HashSha3_224;
zend_class_entry *cryptopp_ce_HashSha3_256;
zend_class_entry *cryptopp_ce_HashSha3_384;
zend_class_entry *cryptopp_ce_HashSha3_512;

static zend_function_entry cryptopp_methods_HashSha3_224[] = {
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_HashSha3_224, __construct, arginfo_HashSha3_224_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    CRYPTOPP_HASH_REQUIRED_METHODS(HashSha3_224)
    PHP_FE_END
};

static zend_function_entry cryptopp_methods_HashSha3_256[] = {
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_HashSha3_256, __construct, arginfo_HashSha3_256_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    CRYPTOPP_HASH_REQUIRED_METHODS(HashSha3_256)
    PHP_FE_END
};

static zend_function_entry cryptopp_methods_HashSha3_384[] = {
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_HashSha3_384, __construct, arginfo_HashSha3_384_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    CRYPTOPP_HASH_REQUIRED_METHODS(HashSha3_384)
    PHP_FE_END
};

static zend_function_entry cryptopp_methods_HashSha3_512[] = {
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_HashSha3_512, __construct, arginfo_HashSha3_512_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    CRYPTOPP_HASH_REQUIRED_METHODS(HashSha3_512)
    PHP_FE_END
};

CRYPTOPP_HASH_INIT_CLASS("sha3_224", HashSha3_224, cryptopp_ce_HashSha3_224, cryptopp_methods_HashSha3_224)
CRYPTOPP_HASH_INIT_CLASS("sha3_256", HashSha3_256, cryptopp_ce_HashSha3_256, cryptopp_methods_HashSha3_256)
CRYPTOPP_HASH_INIT_CLASS("sha3_384", HashSha3_384, cryptopp_ce_HashSha3_384, cryptopp_methods_HashSha3_384)
CRYPTOPP_HASH_INIT_CLASS("sha3_512", HashSha3_512, cryptopp_ce_HashSha3_512, cryptopp_methods_HashSha3_512)

void init_classes_HashSha3(TSRMLS_D)
{
    CRYPTOPP_HASH_INIT_CLASS_FUNC_CALL(HashSha3_224)
    CRYPTOPP_HASH_INIT_CLASS_FUNC_CALL(HashSha3_256)
    CRYPTOPP_HASH_INIT_CLASS_FUNC_CALL(HashSha3_384)
    CRYPTOPP_HASH_INIT_CLASS_FUNC_CALL(HashSha3_512)
}
/* }}} */

/* {{{ proto HashSha3_*::__construct(void) */
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashSha3_224, __construct) {
    CryptoPP::SHA3_224 *hash;
    hash = new CryptoPP::SHA3_224();
    CRYPTOPP_HASH_SET_NATIVE_PTR(HashSha3_224, hash)
}

PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashSha3_256, __construct) {
    CryptoPP::SHA3_256 *hash;
    hash = new CryptoPP::SHA3_256();
    CRYPTOPP_HASH_SET_NATIVE_PTR(HashSha3_256, hash)
}

PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashSha3_384, __construct) {
    CryptoPP::SHA3_384 *hash;
    hash = new CryptoPP::SHA3_384();
    CRYPTOPP_HASH_SET_NATIVE_PTR(HashSha3_384, hash)
}

PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashSha3_512, __construct) {
    CryptoPP::SHA3_512 *hash;
    hash = new CryptoPP::SHA3_512();
    CRYPTOPP_HASH_SET_NATIVE_PTR(HashSha3_512, hash)
}
/* }}} */

/* {{{ proto string HashSha3_*::getName(void)
   Return algorithm name */
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashSha3_224, getName) {
    RETURN_STRING("sha3_224", 1);
}

PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashSha3_256, getName) {
    RETURN_STRING("sha3_256", 1);
}

PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashSha3_384, getName) {
    RETURN_STRING("sha3_384", 1);
}

PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashSha3_512, getName) {
    RETURN_STRING("sha3_512", 1);
}
/* }}} */

/* include common hash methods definitions */
CRYPTOPP_HASH_COMMON_METHODS_DEFINITIONS(HashSha3_224, CryptoPP::SHA3_224)
CRYPTOPP_HASH_COMMON_METHODS_DEFINITIONS(HashSha3_256, CryptoPP::SHA3_256)
CRYPTOPP_HASH_COMMON_METHODS_DEFINITIONS(HashSha3_384, CryptoPP::SHA3_384)
CRYPTOPP_HASH_COMMON_METHODS_DEFINITIONS(HashSha3_512, CryptoPP::SHA3_512)
