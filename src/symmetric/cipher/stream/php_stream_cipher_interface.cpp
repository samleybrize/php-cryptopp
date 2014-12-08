#include "../../../php_cryptopp.h"
#include "php_stream_cipher_interface.h"
#include "../php_symmetric_cipher_interface.h"

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_StreamCipherInterface;

static zend_function_entry cryptopp_methods_StreamCipherInterface[] = {
    PHP_ABSTRACT_ME(Cryptopp_StreamCipherInterface, setKey, arginfo_StreamCipherInterface_setKey)
    PHP_ABSTRACT_ME(Cryptopp_StreamCipherInterface, setIv, arginfo_StreamCipherInterface_setIv)
    PHP_ABSTRACT_ME(Cryptopp_StreamCipherInterface, restart, arginfo_StreamCipherInterface_restart)
    PHP_FE_END
};

void init_interface_StreamCipherInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "StreamCipherInterface", cryptopp_methods_StreamCipherInterface);
    cryptopp_ce_StreamCipherInterface = zend_register_internal_interface(&ce TSRMLS_CC);

    zend_class_implements(cryptopp_ce_StreamCipherInterface TSRMLS_CC, 1, cryptopp_ce_SymmetricCipherInterface);
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
