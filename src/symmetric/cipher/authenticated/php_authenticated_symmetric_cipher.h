#ifndef PHP_AUTHENTICATED_SYMMETRIC_CIPHER_H
#define PHP_AUTHENTICATED_SYMMETRIC_CIPHER_H

#include "src/php_cryptopp.h"
#include "src/utils/algo_list.h"

extern AlgoList authenticatedSymmetricCipherAlgoList;

void init_class_AuthenticatedSymmetricCipher(TSRMLS_D);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipher, getAlgos);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipher, getClassname);

#endif /* PHP_AUTHENTICATED_SYMMETRIC_CIPHER_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
