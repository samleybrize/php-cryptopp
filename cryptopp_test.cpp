#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include <iostream>
#include <string>
#include <cstdio>

#include "php_cryptopp_test.h"

#include "aes.h"
#include "md5.h"
#include "sha.h"

zend_function_entry cryptopp_test_functions[] = {
    PHP_FE(cryptopp_md5, NULL)
    PHP_FE(cryptopp_sha1, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry cryptopp_test_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_CRYPTOPP_TEST_EXTNAME,
    cryptopp_test_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_CRYPTOPP_TEST_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_CRYPTOPP_TEST
extern "C" {
ZEND_GET_MODULE(cryptopp_test)
}
#endif

void outputByteArray(byte *byteArray, int size) {
    for (int i = 0; i < size; i++) {
        php_printf("%ld:", byteArray[i]);
    }

    php_printf("\n");
}

PHP_FUNCTION(cryptopp_md5) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    php_printf("md5 digest size : %ld\n", CryptoPP::Weak::MD5::DIGESTSIZE);

    CryptoPP::Weak::MD5 hash;
    byte digest[CryptoPP::Weak::MD5::DIGESTSIZE];
    hash.CalculateDigest(digest, (byte*) msg, msgSize);

    outputByteArray(digest, sizeof(digest));
}

PHP_FUNCTION(cryptopp_sha1) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    php_printf("sha1 digest size : %ld\n", CryptoPP::SHA::DIGESTSIZE);

    CryptoPP::SHA hash;
    byte digest[CryptoPP::SHA::DIGESTSIZE];
    hash.CalculateDigest(digest, (byte*) msg, msgSize);

    outputByteArray(digest, sizeof(digest));
}

