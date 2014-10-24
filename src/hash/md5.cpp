#include "../php_cryptopp_test.h"
#include "php_hash.h"
#include "php_md5.h"
#include <md5.h>

/*
 * PHP class déclaration
 */
zend_class_entry *cryptopp_test_ce_hash_md5;

static zend_function_entry hash_md5_methods[] = {
    PHP_ME(HashMd5, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(HashMd5, hash, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

void init_class_HashMd5(TSRMLS_D) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "HashMd5", hash_md5_methods);
    cryptopp_test_ce_hash_md5 = zend_register_internal_class(&ce TSRMLS_CC);
}

/*
 * PHP methods definitions
 */
PHP_METHOD(HashMd5, __construct) {
    php_printf("instanciate HashMd5\n");
}

PHP_METHOD(HashMd5, hash) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::Weak::MD5 hash;
    hash(hash, CryptoPP::Weak::MD5::DIGESTSIZE, "md5", (byte*) msg, msgSize)
}

