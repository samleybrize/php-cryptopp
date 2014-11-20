#ifndef PHP_HASH_SHA1_H
#define PHP_HASH_SHA1_H

#include "php_hash_interface.h"

CRYPTOPP_HASH_INIT_CLASS_FUNC_HEADER(HashSha1);
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_HashSha1, __construct);
CRYPTOPP_HASH_REQUIRED_METHODS_HEADER(HashSha1)

#endif /* PHP_HASH_SHA1_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
