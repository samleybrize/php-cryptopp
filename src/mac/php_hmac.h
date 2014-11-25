#ifndef PHP_MAC_HMAC_H
#define PHP_MAC_HMAC_H

#include "php_mac_interface.h"

CRYPTOPP_MAC_INIT_CLASS_FUNC_HEADER(MacHmac);
PHP_METHOD(Cryptopp_MacHmac, __construct);
CRYPTOPP_MAC_REQUIRED_METHODS_HEADER(MacHmac)

#endif /* PHP_MAC_HMAC_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
