#include "../php_cryptopp_test.h"
#include "php_hash_interface.h"
#include "php_sha3.h"
#include <sha3.h>

/*
 * PHP class déclaration
 */
zend_class_entry *cryptopp_test_ce_hash_sha3_256;

static zend_function_entry hash_sha3_256_methods[] = {
    PHP_ME(HashSha3_256, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(HashSha3_256, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(HashSha3_256, hash, NULL, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void init_class_HashSha3(TSRMLS_D) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "HashSha3_256", hash_sha3_256_methods);
    cryptopp_test_ce_hash_sha3_256 = zend_register_internal_class(&ce TSRMLS_CC);
    zend_class_implements(cryptopp_test_ce_hash_sha3_256 TSRMLS_CC, 1, cryptopp_test_ce_hash_interface);
}

/*
 * PHP methods definitions
 */
std::map<long, CryptoPP::SHA3_256> cryptoppBindHashSha3_256;

PHP_METHOD(HashSha3_256, __construct) {
    long handle = Z_OBJ_HANDLE_P(getThis());
    CryptoPP::SHA3_256 hash;
    cryptoppBindHashSha3_256[handle] = hash;

    php_printf("construct HashSha3_256 (handle: %ld)\n", handle); // TODO
    php_printf("map size: %ld\n", cryptoppBindHashSha3_256.size()); // TODO
}

PHP_METHOD(HashSha3_256, __destruct) {
    long handle = Z_OBJ_HANDLE_P(getThis());
    cryptoppBindHashSha3_256.erase(handle);

    php_printf("destruct HashSha3_256 (handle: %ld)\n", handle); // TODO
    php_printf("map size: %ld\n", cryptoppBindHashSha3_256.size()); // TODO
}

PHP_METHOD(HashSha3_256, hash) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    long handle             = Z_OBJ_HANDLE_P(getThis());
    CryptoPP::SHA3_256 hash = cryptoppBindHashSha3_256[handle];

    byte digest[CryptoPP::SHA3_256::DIGESTSIZE];
    hash.CalculateDigest(digest, (byte*) msg, msgSize);

    RETVAL_STRINGL((char*) digest, CryptoPP::SHA3_256::DIGESTSIZE, 1);
}

