#ifndef PHP_PADDING_NO_PADDING_H
#define PHP_PADDING_NO_PADDING_H

#include "src/php_cryptopp.h"

extern zend_class_entry *cryptopp_ce_PaddingNoPadding;
void init_class_PaddingNoPadding(TSRMLS_D);

/* {{{ methods declarations */
PHP_METHOD(Cryptopp_PaddingNoPadding, pad);
PHP_METHOD(Cryptopp_PaddingNoPadding, unpad);
PHP_METHOD(Cryptopp_PaddingNoPadding, canPad);
PHP_METHOD(Cryptopp_PaddingNoPadding, canUnpad);
/* }}} */

#endif /* PHP_PADDING_NO_PADDING_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
