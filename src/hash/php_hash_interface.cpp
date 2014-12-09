#include "../php_cryptopp.h"
#include "php_hash_interface.h"

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_HashInterface;

static zend_function_entry cryptopp_methods_HashInterface[] = {
    PHP_ABSTRACT_ME(Cryptopp_HashInterface, getName, arginfo_HashInterface_getName)
    PHP_ABSTRACT_ME(Cryptopp_HashInterface, getDigestSize, arginfo_HashInterface_getDigestSize)
    PHP_ABSTRACT_ME(Cryptopp_HashInterface, getBlockSize, arginfo_HashInterface_getBlockSize)
    PHP_ABSTRACT_ME(Cryptopp_HashInterface, calculateDigest, arginfo_HashInterface_calculateDigest)
    PHP_ABSTRACT_ME(Cryptopp_HashInterface, update, arginfo_HashInterface_update)
    PHP_ABSTRACT_ME(Cryptopp_HashInterface, final, arginfo_HashInterface_final)
    PHP_ABSTRACT_ME(Cryptopp_HashInterface, restart, arginfo_HashInterface_restart)
    PHP_FE_END
};

void init_interface_HashInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "HashInterface", cryptopp_methods_HashInterface);
    cryptopp_ce_HashInterface = zend_register_internal_interface(&ce TSRMLS_CC);
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
