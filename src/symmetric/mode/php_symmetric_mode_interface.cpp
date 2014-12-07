#include "../../php_cryptopp.h"
#include "php_symmetric_mode_interface.h"
#include "../cipher/php_symmetric_cipher_interface.h"
#include "../cipher/stream/php_stream_cipher_interface.h"

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_SymmetricModeInterface;

static zend_function_entry cryptopp_methods_SymmetricModeInterface[] = {
    PHP_ABSTRACT_ME(Cryptopp_SymmetricModeInterface, getName, arginfo_SymmetricCipherInterface_getName)
    PHP_ABSTRACT_ME(Cryptopp_SymmetricModeInterface, setKey, arginfo_StreamCipherInterface_setKey)
    PHP_ABSTRACT_ME(Cryptopp_SymmetricModeInterface, setIv, arginfo_StreamCipherInterface_setIv)
    PHP_FE_END
};

void init_interface_SymmetricModeInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "SymmetricModeInterface", cryptopp_methods_SymmetricModeInterface);
    cryptopp_ce_SymmetricModeInterface = zend_register_internal_interface(&ce TSRMLS_CC);

    zend_class_implements(cryptopp_ce_SymmetricModeInterface TSRMLS_CC, 1, cryptopp_ce_StreamCipherInterface);
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
