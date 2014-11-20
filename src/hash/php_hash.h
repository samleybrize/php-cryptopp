#ifndef PHP_HASH_H
#define PHP_HASH_H

#include "../php_cryptopp.h"
#include <string>
#include <vector>

void addHashAlgo(const std::string algoName, const std::string hashClassname);
std::vector<std::string> getHashAlgoList();

void init_class_Hash(TSRMLS_D);
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_Hash, getAlgos);
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_Hash, getClassname);

#endif /* PHP_HASH_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
