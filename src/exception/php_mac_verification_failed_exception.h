#ifndef PHP_CRYPTOPP_MAC_VERIFICATION_FAILED_EXCEPTION_H
#define PHP_CRYPTOPP_MAC_VERIFICATION_FAILED_EXCEPTION_H

#include "src/php_cryptopp.h"

void init_class_CryptoppMacVerificationFailedException(TSRMLS_D);
zend_class_entry *getCryptoppMacVerificationFailedException();

#endif /* PHP_CRYPTOPP_MAC_VERIFICATION_FAILED_EXCEPTION_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
