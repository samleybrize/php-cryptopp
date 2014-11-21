#ifndef PHP_HASH_MD5_H
#define PHP_HASH_MD5_H

#include "php_hash_interface.h"

CRYPTOPP_HASH_INIT_CLASS_FUNC_HEADER(HashMd5);
PHP_METHOD(Cryptopp_HashMd5, __construct);
CRYPTOPP_HASH_REQUIRED_METHODS_HEADER(HashMd5)

#endif /* PHP_HASH_MD5_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
