#ifndef PHP_SYMMETRIC_CIPHER_AES_H
#define PHP_SYMMETRIC_CIPHER_AES_H

#include "php_symmetric_cipher_interface.h"

CRYPTOPP_SYMMETRIC_CIPHER_INIT_CLASS_FUNC_HEADER(SymmetricCipherAes);
PHP_METHOD(Cryptopp_SymmetricCipherAes, __construct);
CRYPTOPP_SYMMETRIC_CIPHER_REQUIRED_METHODS_HEADER(SymmetricCipherAes)

#endif /* PHP_SYMMETRIC_CIPHER_AES_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
