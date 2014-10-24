#ifndef HASH_MD5_H
#define HASH_MD5_H

extern "C" {
#include "php.h"
}

void init_class_HashMd5(TSRMLS_D);
PHP_METHOD(HashMd5, __construct);
PHP_METHOD(HashMd5, hash);

#endif

