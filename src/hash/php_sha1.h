#ifndef HASH_SHA1_H
#define HASH_SHA1_H

extern "C" {
#include "php.h"
}

void init_class_HashSha1(TSRMLS_D);
PHP_METHOD(HashSha1, __construct);
PHP_METHOD(HashSha1, __destruct);
PHP_METHOD(HashSha1, hash);

#endif

