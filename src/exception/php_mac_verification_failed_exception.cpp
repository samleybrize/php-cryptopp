#include "../php_cryptopp.h"
#include "php_exception.h"
#include "php_mac_verification_failed_exception.h"
#include <zend_exceptions.h>

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_CryptoppMacVerificationFailedException;

void init_class_CryptoppMacVerificationFailedException(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "CryptoppMacVerificationFailedException", NULL);
    cryptopp_ce_CryptoppMacVerificationFailedException = zend_register_internal_class_ex(&ce, getCryptoppException(), NULL TSRMLS_CC);
}
/* }}} */

/* {{{ return a pointer to the CryptoppMacVerificationFailedException class entry */
zend_class_entry *getCryptoppMacVerificationFailedException() {
    return cryptopp_ce_CryptoppMacVerificationFailedException;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
