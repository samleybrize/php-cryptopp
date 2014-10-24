#include "../php_cryptopp_test.h"
#include "php_hash.h"
#include "php_sha3.h"
#include <sha3.h>


PHP_FUNCTION(cryptopp_sha3_256) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::SHA3_256 hash;
    hash(hash, CryptoPP::SHA3_256::DIGESTSIZE, "sha3_256", (byte*) msg, msgSize)
}

