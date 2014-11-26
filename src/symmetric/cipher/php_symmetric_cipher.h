#ifndef PHP_SYMMETRIC_CIPHER_H
#define PHP_SYMMETRIC_CIPHER_H

#include "../../php_cryptopp.h"
#include <string>
#include <vector>

void addSymmetricCipherAlgo(const std::string algoName, const std::string cipherClassname);
std::vector<std::string> getSymmetricCipherAlgoList();

void init_class_SymmetricCipher(TSRMLS_D);
PHP_METHOD(Cryptopp_SymmetricCipher, getAlgos);
PHP_METHOD(Cryptopp_SymmetricCipher, getClassname);

#endif /* PHP_SYMMETRIC_CIPHER_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
