#include "../php_cryptopp.h"
#include "php_hash_interface.h"

zend_class_entry *cryptopp_ce_HashInterface;

static zend_function_entry cryptopp_methods_HashInterface[] = {
    PHP_ABSTRACT_ME(PHP_CRYPTOPP_NAMESPACE_HashInterface, getName, arginfo_HashInterface_getName)
    PHP_ABSTRACT_ME(PHP_CRYPTOPP_NAMESPACE_HashInterface, getDigestSize, arginfo_HashInterface_getDigestSize)
    PHP_ABSTRACT_ME(PHP_CRYPTOPP_NAMESPACE_HashInterface, calculateDigest, arginfo_HashInterface_calculateDigest)
    PHP_FE_END
};

void init_interface_HashInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, PHP_CRYPTOPP_NAMESPACE, "HashInterface", cryptopp_methods_HashInterface);
    cryptopp_ce_HashInterface = zend_register_internal_interface(&ce TSRMLS_CC);
}
