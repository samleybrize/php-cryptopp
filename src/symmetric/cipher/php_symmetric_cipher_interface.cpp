#include "src/php_cryptopp.h"
#include "php_symmetric_cipher_interface.h"

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_SymmetricCipherInterface;

static zend_function_entry cryptopp_methods_SymmetricCipherInterface[] = {
    PHP_ABSTRACT_ME(Cryptopp_SymmetricCipherInterface, getName, arginfo_SymmetricCipherInterface_getName)
    PHP_ABSTRACT_ME(Cryptopp_SymmetricCipherInterface, getBlockSize, arginfo_SymmetricCipherInterface_getBlockSize)
    PHP_ABSTRACT_ME(Cryptopp_SymmetricCipherInterface, isValidKeyLength, arginfo_SymmetricCipherInterface_isValidKeyLength)
    PHP_ABSTRACT_ME(Cryptopp_SymmetricCipherInterface, setKey, arginfo_SymmetricCipherInterface_setKey)
    PHP_ABSTRACT_ME(Cryptopp_SymmetricCipherInterface, getKey, arginfo_SymmetricCipherInterface_getKey)
    PHP_FE_END
};

void init_interface_SymmetricCipherInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "SymmetricCipherInterface", cryptopp_methods_SymmetricCipherInterface);
    cryptopp_ce_SymmetricCipherInterface = zend_register_internal_interface(&ce TSRMLS_CC);
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
