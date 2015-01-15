#ifndef PHP_SYMMETRIC_MODE_H
#define PHP_SYMMETRIC_MODE_H

#include "src/php_cryptopp.h"
#include "src/utils/algo_list.h"

extern AlgoList symmetricModeList;

void init_class_SymmetricMode(TSRMLS_D);
PHP_METHOD(Cryptopp_SymmetricMode, getModes);
PHP_METHOD(Cryptopp_SymmetricMode, getClassname);

#endif /* PHP_SYMMETRIC_MODE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
