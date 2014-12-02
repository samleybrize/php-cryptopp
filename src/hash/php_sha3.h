#ifndef PHP_HASH_SHA3_H
#define PHP_HASH_SHA3_H

#include "../php_cryptopp.h"

void init_classes_HashSha3(TSRMLS_D);
PHP_METHOD(Cryptopp_HashSha3_224, __construct);
PHP_METHOD(Cryptopp_HashSha3_256, __construct);
PHP_METHOD(Cryptopp_HashSha3_384, __construct);
PHP_METHOD(Cryptopp_HashSha3_512, __construct);

#endif /* PHP_HASH_SHA3_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
