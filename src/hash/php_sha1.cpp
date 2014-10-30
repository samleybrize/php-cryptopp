#include "../php_cryptopp.h"
#include "php_hash_interface.h"
#include "php_sha1.h"
#include <sha.h>

// PHP class args info
ZEND_BEGIN_ARG_INFO(arginfo_HashSha1_construct, 0)
ZEND_END_ARG_INFO()

// PHP class déclaration
zend_class_entry *cryptopp_ce_HashSha1;

static zend_function_entry cryptopp_methods_HashSha1[] = {
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_HashSha1, __construct, arginfo_HashSha1_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    CRYPTOPP_HASH_REQUIRED_METHODS(HashSha1)
    PHP_FE_END
};

CRYPTOPP_HASH_INIT_CLASS("sha1", HashSha1, CryptoPP::SHA1, cryptopp_ce_HashSha1, cryptopp_methods_HashSha1)

// PHP methods definitions
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashSha1, __construct) {
    CryptoPP::SHA1 *hash;
    hash = new CryptoPP::SHA1();
    CRYPTOPP_HASH_SET_NATIVE_PTR(HashSha1, hash)
}

PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashSha1, getName) {
    RETURN_STRING("sha1", 1);
}

CRYPTOPP_HASH_REQUIRED_METHODS_DEFINITIONS(HashSha1, CryptoPP::SHA1)
