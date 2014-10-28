#ifndef HASH_SHA3_H
#define HASH_SHA3_H

#include "php_hash_interface.h"

extern "C" {
#include "php.h"
}

void init_class_HashSha3(TSRMLS_D);
PHP_METHOD(HashSha3_256, __construct);
CRYPTOPP_HASH_GET_REQUIRED_METHODS_HEADER(HashSha3_256)

#endif
