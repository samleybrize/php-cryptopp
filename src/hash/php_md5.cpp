#include "../php_cryptopp.h"
#include "php_hash_interface.h"
#include "php_md5.h"
#include <md5.h>

// PHP class déclaration
zend_class_entry *cryptopp_ce_HashMd5;

static zend_function_entry cryptopp_methods_HashMd5[] = {
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_HashMd5, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    CRYPTOPP_HASH_REQUIRED_METHODS(HashMd5)
    PHP_FE_END
};

CRYPTOPP_HASH_INIT_CLASS(HashMd5, CryptoPP::Weak::MD5, cryptopp_ce_HashMd5, cryptopp_methods_HashMd5)

// PHP methods definitions
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashMd5, __construct) {
    CryptoPP::Weak::MD5 *hash;
    hash = new CryptoPP::Weak::MD5();
    CRYPTOPP_HASH_SET_NATIVE_PTR(HashMd5, hash)
}

CRYPTOPP_HASH_REQUIRED_METHODS_DEFINITIONS(HashMd5, CryptoPP::Weak::MD5)
