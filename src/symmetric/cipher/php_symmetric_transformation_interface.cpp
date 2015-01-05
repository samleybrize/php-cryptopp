#include "../../php_cryptopp.h"
#include "php_symmetric_cipher_interface.h"
#include "php_symmetric_transformation_interface.h"

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_SymmetricTransformationInterface;

static zend_function_entry cryptopp_methods_SymmetricTransformationInterface[] = {
    PHP_ABSTRACT_ME(Cryptopp_SymmetricTransformationInterface, setIv, arginfo_SymmetricTransformationInterface_setIv)
    PHP_ABSTRACT_ME(Cryptopp_SymmetricTransformationInterface, isValidIvLength, arginfo_SymmetricTransformationInterface_isValidIvLength)
    PHP_ABSTRACT_ME(Cryptopp_SymmetricTransformationInterface, encrypt, arginfo_SymmetricTransformationInterface_encrypt)
    PHP_ABSTRACT_ME(Cryptopp_SymmetricTransformationInterface, decrypt, arginfo_SymmetricTransformationInterface_decrypt)
    PHP_ABSTRACT_ME(Cryptopp_SymmetricTransformationInterface, restart, arginfo_SymmetricTransformationInterface_restart)
    PHP_FE_END
};

void init_interface_SymmetricTransformationInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "SymmetricTransformationInterface", cryptopp_methods_SymmetricTransformationInterface);
    cryptopp_ce_SymmetricTransformationInterface = zend_register_internal_interface(&ce TSRMLS_CC);

    zend_class_implements(cryptopp_ce_SymmetricTransformationInterface TSRMLS_CC, 1, cryptopp_ce_SymmetricCipherInterface);
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
