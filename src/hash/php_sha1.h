#ifndef HASH_SHA1_H
#define HASH_SHA1_H

#include "php_hash_interface.h"

extern "C" {
#include "php.h"
}

CRYPTOPP_HASH_GET_INIT_CLASS_FUNC_HEADER(HashSha1);
PHP_METHOD(HashSha1, __construct);
CRYPTOPP_HASH_GET_REQUIRED_METHODS_HEADER(HashSha1)

#endif
