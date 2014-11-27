#ifndef PHP_SYMMETRIC_MODE_ECB_H
#define PHP_SYMMETRIC_MODE_ECB_H

#include "php_symmetric_mode_interface.h"

CRYPTOPP_SYMMETRIC_MODE_INIT_CLASS_FUNC_HEADER(SymmetricModeEcb);
PHP_METHOD(Cryptopp_SymmetricModeEcb, __construct);
CRYPTOPP_SYMMETRIC_MODE_REQUIRED_METHODS_HEADER(SymmetricModeEcb)

#endif /* PHP_SYMMETRIC_MODE_ECB_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
