#ifndef PHP_BLOCK_CIPHER_H
#define PHP_BLOCK_CIPHER_H

#include "../../../php_cryptopp.h"
#include "../../../utils/algo_list.h"

extern AlgoList blockCipherAlgoList;

void init_class_BlockCipher(TSRMLS_D);
PHP_METHOD(Cryptopp_BlockCipher, getAlgos);
PHP_METHOD(Cryptopp_BlockCipher, getClassname);

#endif /* PHP_BLOCK_CIPHER_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
