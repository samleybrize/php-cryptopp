#include "../php_cryptopp_test.h"
#include "php_hash_interface.h"

// TODO arg info
zend_class_entry *cryptopp_test_ce_hash_interface;

static zend_function_entry hash_interface_methods[] = {
    PHP_ABSTRACT_ME(HashInterface, hash, NULL)
    PHP_FE_END
};

void init_interface_HashInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "HashInterface", hash_interface_methods);
    cryptopp_test_ce_hash_interface = zend_register_internal_interface(&ce TSRMLS_CC);
}

