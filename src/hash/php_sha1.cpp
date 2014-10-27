#include "../php_cryptopp_test.h"
#include "php_hash_interface.h"
#include "php_sha1.h"
#include <sha.h>
#include <map>

/*
 * PHP class déclaration
 */
zend_class_entry *cryptopp_test_ce_hash_sha1;

static zend_function_entry hash_sha1_methods[] = {
    PHP_ME(HashSha1, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(HashSha1, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(HashSha1, hash, NULL, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void init_class_HashSha1(TSRMLS_D) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "HashSha1", hash_sha1_methods);
    cryptopp_test_ce_hash_sha1 = zend_register_internal_class(&ce TSRMLS_CC);
    zend_class_implements(cryptopp_test_ce_hash_sha1 TSRMLS_CC, 1, cryptopp_test_ce_hash_interface);
}

/*
 * PHP methods definitions
 */
std::map<long, CryptoPP::SHA1> cryptoppBindHashSha1;

PHP_METHOD(HashSha1, __construct) {
    long handle = Z_OBJ_HANDLE_P(getThis());
    CryptoPP::SHA1 hash;
    cryptoppBindHashSha1[handle] = hash;
}

PHP_METHOD(HashSha1, __destruct) {
    long handle = Z_OBJ_HANDLE_P(getThis());
    cryptoppBindHashSha1.erase(handle);
}

PHP_METHOD(HashSha1, hash) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    long handle         = Z_OBJ_HANDLE_P(getThis());
    CryptoPP::SHA1 hash = cryptoppBindHashSha1[handle];

    byte digest[CryptoPP::SHA1::DIGESTSIZE];
    hash.CalculateDigest(digest, (byte*) msg, msgSize);

    RETVAL_STRINGL((char*) digest, CryptoPP::SHA1::DIGESTSIZE, 1);
}

