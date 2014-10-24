#ifndef HASH_SHA3_H
#define HASH_SHA3_H

extern "C" {
#include "php.h"
}

void init_class_HashSha3(TSRMLS_D);
PHP_METHOD(HashSha3_256, __construct);
PHP_METHOD(HashSha3_256, __destruct);
PHP_METHOD(HashSha3_256, hash);

#endif

