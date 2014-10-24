#include "../php_cryptopp_test.h"
#include "php_hash.h"
#include "php_sha1.h"
#include <sha.h>

/*
 * PHP class déclaration
 */
zend_class_entry *cryptopp_test_ce_hash_sha1;

static zend_function_entry hash_sha1_methods[] = {
    PHP_ME(HashSha1, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(HashSha1, hash, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

void init_class_HashSha1(TSRMLS_D) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "HashSha1", hash_sha1_methods);
    cryptopp_test_ce_hash_sha1 = zend_register_internal_class(&ce TSRMLS_CC);
}

/*
 * PHP methods definitions
 */
PHP_METHOD(HashSha1, __construct) {
    php_printf("instanciate HashSha1\n");
}

PHP_METHOD(HashSha1, hash) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::SHA1 hash;
    hash(hash, CryptoPP::SHA1::DIGESTSIZE, "sha1", (byte*) msg, msgSize)
}


