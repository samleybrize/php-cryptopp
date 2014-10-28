#include "../php_cryptopp.h"
#include "php_hash_interface.h"

// TODO arg info
zend_class_entry *cryptopp_ce_HashInterface;

static zend_function_entry cryptopp_methods_HashInterface[] = {
    PHP_ABSTRACT_ME(HashInterface, hash, NULL)
    PHP_FE_END
};

void init_interface_HashInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "HashInterface", cryptopp_methods_HashInterface);
    cryptopp_ce_HashInterface = zend_register_internal_interface(&ce TSRMLS_CC);
}
