#ifndef PHP_RBG_H
#define PHP_RBG_H

#include "php_rbg_interface.h"

CRYPTOPP_RBG_INIT_CLASS_FUNC_HEADER(RandomByteGenerator);
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_RandomByteGenerator, __construct);
CRYPTOPP_RBG_REQUIRED_METHODS_HEADER(RandomByteGenerator)

#endif /* PHP_RBG_H */
