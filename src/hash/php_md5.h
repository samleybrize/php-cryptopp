#ifndef PHP_HASH_MD5_H
#define PHP_HASH_MD5_H

#include "../php_cryptopp.h"

void init_class_HashMd5(TSRMLS_D);
PHP_METHOD(Cryptopp_HashMd5, __construct);

#endif /* PHP_HASH_MD5_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
