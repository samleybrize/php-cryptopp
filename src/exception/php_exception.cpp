#include "../php_cryptopp.h"
#include "php_exception.h"
#include <zend_exceptions.h>

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_CryptoPPException;

void init_class_CryptoPPException(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, PHP_CRYPTOPP_NAMESPACE, "CryptoPPException", NULL);
    cryptopp_ce_CryptoPPException = zend_register_internal_class_ex(&ce, (zend_class_entry*)zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
}
/* }}} */

/* {{{ return a pointer to the CryptoPPException class entry */
zend_class_entry *getCryptoPPException() {
    return cryptopp_ce_CryptoPPException;
}
/* }}} */
