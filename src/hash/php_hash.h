#ifndef PHP_HASH_H
#define PHP_HASH_H

#include "../php_cryptopp.h"

void addHashAlgo(const std::string algoName, const std::string hashClassname);

void init_class_Hash(TSRMLS_D);
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_Hash, getAlgos);
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_Hash, getClassname);

#endif /* PHP_HASH_H */
