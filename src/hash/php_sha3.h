#ifndef HASH_SHA3_H
#define HASH_SHA3_H

#include "php_hash_interface.h"

void init_classes_HashSha3(TSRMLS_D);
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashSha3_256, __construct);
CRYPTOPP_HASH_REQUIRED_METHODS_HEADER(HashSha3_256)

#endif /* HASH_SHA3_H */
