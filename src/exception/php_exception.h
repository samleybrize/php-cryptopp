#ifndef PHP_CRYPTOPP_EXCEPTION_H
#define PHP_CRYPTOPP_EXCEPTION_H

#include "../php_cryptopp.h"

void init_class_CryptoppException(TSRMLS_D);
zend_class_entry *getCryptoppException();

#endif /* PHP_CRYPTOPP_EXCEPTION_H */
