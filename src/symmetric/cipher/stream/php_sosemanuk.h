#ifndef PHP_STREAM_CIPHER_SOSEMANUK_H
#define PHP_STREAM_CIPHER_SOSEMANUK_H

#include "src/php_cryptopp.h"

void init_class_StreamCipherSosemanuk(TSRMLS_D);
PHP_METHOD(Cryptopp_StreamCipherSosemanuk, __construct);

#endif /* PHP_STREAM_CIPHER_SOSEMANUK_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
