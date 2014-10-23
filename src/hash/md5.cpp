#include "../php_cryptopp_test.h"
#include "php_hash.h"
#include "php_md5.h"
#include <md5.h>

PHP_FUNCTION(cryptopp_md5) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::Weak::MD5 hash;
    hash(hash, CryptoPP::Weak::MD5::DIGESTSIZE, "md5", (byte*) msg, msgSize)
}

