#ifndef PHP_HASH_H
#define PHP_HASH_H

#include "src/php_cryptopp.h"
#include "src/utils/algo_list.h"

extern AlgoList hashAlgoList;

void init_class_Hash(TSRMLS_D);
PHP_METHOD(Cryptopp_Hash, getAlgos);
PHP_METHOD(Cryptopp_Hash, getClassname);

#endif /* PHP_HASH_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
