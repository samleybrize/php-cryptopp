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
    PHP_ME(HashSha1, hash, NULL, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

CRYPTOPP_HASH_INIT_CLASS(HashSha1, CryptoPP::SHA1, cryptopp_test_ce_hash_sha1, hash_sha1_methods)

/*
 * PHP methods definitions
 */
PHP_METHOD(HashSha1, __construct) {
    CryptoPP::SHA1 *hash;
    hash = new CryptoPP::SHA1();
    CRYPTOPP_HASH_SET_NATIVE_PTR(HashSha1, hash)
}

PHP_METHOD(HashSha1, hash) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::SHA1 *hash;
    hash = CRYPTOPP_HASH_GET_NATIVE_PTR(HashSha1);

    byte digest[CryptoPP::SHA1::DIGESTSIZE];
    hash->CalculateDigest(digest, (byte*) msg, msgSize);

    RETVAL_STRINGL((char*) digest, CryptoPP::SHA1::DIGESTSIZE, 1);
}
