#ifndef PHP_MAC_H
#define PHP_MAC_H

#include "../php_cryptopp.h"
#include <string>
#include <vector>

void addMacAlgo(const std::string algoName, const std::string macClassname);
std::vector<std::string> getMacAlgoList();

void init_class_Mac(TSRMLS_D);
PHP_METHOD(Cryptopp_Mac, getAlgos);
PHP_METHOD(Cryptopp_Mac, getClassname);

#endif /* PHP_MAC_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
