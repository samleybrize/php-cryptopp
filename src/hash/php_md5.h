#ifndef HASH_MD5_H
#define HASH_MD5_H

#include "php_hash_interface.h"

extern "C" {
#include "php.h"
}

CRYPTOPP_HASH_GET_INIT_CLASS_FUNC_HEADER(HashMd5);
PHP_METHOD(HashMd5, __construct);
CRYPTOPP_HASH_GET_REQUIRED_METHODS_HEADER(HashMd5)

#endif
