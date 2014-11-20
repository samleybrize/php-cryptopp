#ifndef PHP_RBG_H
#define PHP_RBG_H

#include "php_rbg_interface.h"

CRYPTOPP_RBG_INIT_CLASS_FUNC_HEADER(RandomByteGenerator);
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_RandomByteGenerator, __construct);
CRYPTOPP_RBG_REQUIRED_METHODS_HEADER(RandomByteGenerator)

#endif /* PHP_RBG_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
