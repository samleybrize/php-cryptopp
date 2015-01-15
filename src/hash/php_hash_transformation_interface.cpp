#include "src/php_cryptopp.h"
#include "php_hash_transformation_interface.h"

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_HashTransformationInterface;

static zend_function_entry cryptopp_methods_HashTransformationInterface[] = {
    PHP_ABSTRACT_ME(Cryptopp_HashTransformationInterface, getName, arginfo_HashTransformationInterface_getName)
    PHP_ABSTRACT_ME(Cryptopp_HashTransformationInterface, getDigestSize, arginfo_HashTransformationInterface_getDigestSize)
    PHP_ABSTRACT_ME(Cryptopp_HashTransformationInterface, getBlockSize, arginfo_HashTransformationInterface_getBlockSize)
    PHP_ABSTRACT_ME(Cryptopp_HashTransformationInterface, calculateDigest, arginfo_HashTransformationInterface_calculateDigest)
    PHP_ABSTRACT_ME(Cryptopp_HashTransformationInterface, update, arginfo_HashTransformationInterface_update)
    PHP_ABSTRACT_ME(Cryptopp_HashTransformationInterface, finalize, arginfo_HashTransformationInterface_finalize)
    PHP_ABSTRACT_ME(Cryptopp_HashTransformationInterface, restart, arginfo_HashTransformationInterface_restart)
    PHP_FE_END
};

void init_interface_HashTransformationInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "HashTransformationInterface", cryptopp_methods_HashTransformationInterface);
    cryptopp_ce_HashTransformationInterface = zend_register_internal_interface(&ce TSRMLS_CC);
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
