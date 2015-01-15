#include "src/php_cryptopp.h"
#include "php_padding_interface.h"

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_PaddingInterface;

static zend_function_entry cryptopp_methods_PaddingInterface[] = {
    PHP_ABSTRACT_ME(Cryptopp_PaddingInterface, pad, arginfo_PaddingInterface_pad)
    PHP_ABSTRACT_ME(Cryptopp_PaddingInterface, unpad, arginfo_PaddingInterface_unpad)
    PHP_ABSTRACT_ME(Cryptopp_PaddingInterface, canPad, arginfo_PaddingInterface_canPad)
    PHP_ABSTRACT_ME(Cryptopp_PaddingInterface, canUnpad, arginfo_PaddingInterface_canUnpad)
    PHP_FE_END
};

void init_interface_PaddingInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "PaddingInterface", cryptopp_methods_PaddingInterface);
    cryptopp_ce_PaddingInterface = zend_register_internal_interface(&ce TSRMLS_CC);
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
