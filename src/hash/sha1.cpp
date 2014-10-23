#include "../php_cryptopp_test.h"
#include "sha1.h"
#include "hash.h"
#include <sha.h>


PHP_FUNCTION(cryptopp_sha1) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::SHA1 hash;
    hash(hash, CryptoPP::SHA1::DIGESTSIZE, "sha1", (byte*) msg, msgSize)
}

