#ifndef PHP_SYMMETRIC_CIPHER_AES_H
#define PHP_SYMMETRIC_CIPHER_AES_H

#include "../../php_cryptopp.h"

void init_class_SymmetricCipherAes(TSRMLS_D);
PHP_METHOD(Cryptopp_SymmetricCipherAes, __construct);

#endif /* PHP_SYMMETRIC_CIPHER_AES_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
