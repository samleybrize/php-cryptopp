#include "../php_cryptopp.h"
#include "php_padding_interface.h"
#include "php_no_padding.h"

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_PaddingNoPadding;

static zend_function_entry cryptopp_methods_PaddingNoPadding[] = {
    PHP_ME(Cryptopp_PaddingNoPadding, pad, arginfo_PaddingInterface_pad, ZEND_ACC_PUBLIC)
    PHP_ME(Cryptopp_PaddingNoPadding, unpad, arginfo_PaddingInterface_unpad, ZEND_ACC_PUBLIC)
    PHP_ME(Cryptopp_PaddingNoPadding, canPad, arginfo_PaddingInterface_canPad, ZEND_ACC_PUBLIC)
    PHP_ME(Cryptopp_PaddingNoPadding, canUnpad, arginfo_PaddingInterface_canUnpad, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void init_class_PaddingNoPadding(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "PaddingNoPadding", cryptopp_methods_PaddingNoPadding);
    cryptopp_ce_PaddingNoPadding = zend_register_internal_class(&ce TSRMLS_CC);

    zend_class_implements(cryptopp_ce_PaddingNoPadding TSRMLS_CC, 1, cryptopp_ce_PaddingInterface);
}
/* }}} */

/* {{{ proto string|false PaddingNoPadding::pad(string data, int blockSize)
   Pad data */
PHP_METHOD(Cryptopp_PaddingNoPadding, pad) {
    char *data;
    int dataLength;
    long blockSize;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &data, &dataLength, &blockSize)) {
        RETURN_FALSE;
    }

    RETURN_STRINGL(data, dataLength, 1)
}
/* }}} */

/* {{{ proto string|false PaddingNoPadding::unpad(string data, int blockSize)
   Unpad data */
PHP_METHOD(Cryptopp_PaddingNoPadding, unpad) {
    char *data;
    int dataLength;
    long blockSize;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &data, &dataLength, &blockSize)) {
        RETURN_FALSE;
    }

    RETURN_STRINGL(data, dataLength, 1)
}
/* }}} */

/* {{{ proto boolean PaddingNoPadding::canPad(void)
   Indicates if this padding scheme can pad data */
PHP_METHOD(Cryptopp_PaddingNoPadding, canPad) {
    RETURN_FALSE
}
/* }}} */

/* {{{ proto boolean PaddingNoPadding::canUnpad(void)
   Indicates if this padding scheme can unpad data */
PHP_METHOD(Cryptopp_PaddingNoPadding, canUnpad) {
    RETURN_FALSE
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
