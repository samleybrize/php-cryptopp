#include "../php_cryptopp.h"
#include "php_mac_interface.h"

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_MacInterface;

static zend_function_entry cryptopp_methods_MacInterface[] = {
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, getName, arginfo_MacInterface_getName)
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, getDigestSize, arginfo_MacInterface_getDigestSize)
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, setKey, arginfo_MacInterface_setKey)
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, calculateDigest, arginfo_MacInterface_calculateDigest)
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, update, arginfo_MacInterface_update)
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, final, arginfo_MacInterface_final)
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, restart, arginfo_MacInterface_restart)
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, verify, arginfo_MacInterface_verify)
    PHP_FE_END
};

void init_interface_MacInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "MacInterface", cryptopp_methods_MacInterface);
    cryptopp_ce_MacInterface = zend_register_internal_interface(&ce TSRMLS_CC);
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
