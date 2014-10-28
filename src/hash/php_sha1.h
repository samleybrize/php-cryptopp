#ifndef HASH_SHA1_H
#define HASH_SHA1_H

#include "php_hash_interface.h"

extern "C" {
#include "php.h"
}

void init_class_HashSha1(TSRMLS_D);
PHP_METHOD(HashSha1, __construct);
CRYPTOPP_HASH_GET_REQUIRED_METHODS_HEADER(HashSha1)

#endif
