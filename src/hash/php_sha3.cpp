#include "../php_cryptopp.h"
#include "php_hash_interface.h"
#include "php_sha3.h"
#include <sha3.h>

/*
 * PHP class déclaration
 */
zend_class_entry *cryptopp_ce_HashSha3_256;

static zend_function_entry cryptopp_methods_HashSha3_256[] = {
    PHP_ME(HashSha3_256, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    CRYPTOPP_HASH_REQUIRED_METHODS(HashSha3_256)
    PHP_FE_END
};

CRYPTOPP_HASH_INIT_CLASS(HashSha3_256, CryptoPP::SHA3_256, cryptopp_ce_HashSha3_256, cryptopp_methods_HashSha3_256)

void init_classes_HashSha3(TSRMLS_D)
{
    CRYPTOPP_HASH_INIT_CLASS_FUNC_CALL(HashSha3_256)
}

/*
 * PHP methods definitions
 */
PHP_METHOD(HashSha3_256, __construct) {
    CryptoPP::SHA3_256 *hash;
    hash = new CryptoPP::SHA3_256();
    CRYPTOPP_HASH_SET_NATIVE_PTR(HashSha3_256, hash)
}

CRYPTOPP_HASH_REQUIRED_METHODS_DEFINITIONS(HashSha3_256, CryptoPP::SHA3_256)
