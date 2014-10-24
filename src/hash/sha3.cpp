#include "../php_cryptopp_test.h"
#include "php_hash.h"
#include "php_sha3.h"
#include <sha3.h>

/*
 * PHP class déclaration
 */
zend_class_entry *cryptopp_test_ce_hash_sha3_256;

static zend_function_entry hash_sha3_256_methods[] = {
    PHP_ME(HashSha3_256, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(HashSha3_256, hash, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

void init_class_HashSha3(TSRMLS_D) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "HashSha3_256", hash_sha3_256_methods);
    cryptopp_test_ce_hash_sha3_256 = zend_register_internal_class(&ce TSRMLS_CC);
}

/*
 * PHP methods definitions
 */
PHP_METHOD(HashSha3_256, __construct) {
    php_printf("instanciate HashSha3_256\n");
}

PHP_METHOD(HashSha3_256, hash) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::SHA3_256 hash;
    hash(hash, CryptoPP::SHA3_256::DIGESTSIZE, "sha3_256", (byte*) msg, msgSize)
}

