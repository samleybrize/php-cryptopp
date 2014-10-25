#include "../php_cryptopp_test.h"
#include "php_hash_interface.h"
#include "php_md5.h"
#include <md5.h>
#include <map>

/*
 * PHP class déclaration
 */
zend_class_entry *cryptopp_test_ce_hash_md5;

static zend_function_entry hash_md5_methods[] = {
    PHP_ME(HashMd5, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(HashMd5, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(HashMd5, hash, NULL, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void init_class_HashMd5(TSRMLS_D) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "HashMd5", hash_md5_methods);
    cryptopp_test_ce_hash_md5 = zend_register_internal_class(&ce TSRMLS_CC);
    zend_class_implements(cryptopp_test_ce_hash_md5 TSRMLS_CC, 1, cryptopp_test_ce_hash_interface);
}

/*
 * PHP methods definitions
 */
std::map<long, CryptoPP::Weak::MD5> cryptoppBindHashMd5;

PHP_METHOD(HashMd5, __construct) {
    long handle = Z_OBJ_HANDLE_P(getThis());
    CryptoPP::Weak::MD5 hash;
    cryptoppBindHashMd5[handle] = hash;

    php_printf("construct HashMd5 (handle: %ld)\n", handle); // TODO
    php_printf("map size: %ld\n", cryptoppBindHashMd5.size()); // TODO
}

PHP_METHOD(HashMd5, __destruct) {
    long handle = Z_OBJ_HANDLE_P(getThis());
    cryptoppBindHashMd5.erase(handle);

    php_printf("destruct HashMd5 (handle: %ld)\n", handle); // TODO
    php_printf("map size: %ld\n", cryptoppBindHashMd5.size()); // TODO
}

PHP_METHOD(HashMd5, hash) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    long handle              = Z_OBJ_HANDLE_P(getThis());
    CryptoPP::Weak::MD5 hash = cryptoppBindHashMd5[handle];

    byte digest[CryptoPP::Weak::MD5::DIGESTSIZE];
    hash.CalculateDigest(digest, (byte*) msg, msgSize);

    RETVAL_STRINGL((char*) digest, CryptoPP::Weak::MD5::DIGESTSIZE, 1);
}

