#ifndef HASH_MD5_H
#define HASH_MD5_H

#include "php_hash_interface.h"

extern "C" {
#include "php.h"
}

void init_class_HashMd5(TSRMLS_D);
PHP_METHOD(HashMd5, __construct);
CRYPTOPP_HASH_GET_REQUIRED_METHODS_HEADER(HashMd5)

#endif

