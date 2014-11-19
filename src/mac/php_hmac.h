#ifndef PHP_MAC_HMAC_H
#define PHP_MAC_HMAC_H

#include "php_mac_interface.h"

void init_class_MacHmac_(TSRMLS_D);
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_MacHmac, __construct);
CRYPTOPP_MAC_REQUIRED_METHODS_HEADER(MacHmac)

#endif /* PHP_MAC_HMAC_H */
