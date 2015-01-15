#include "src/php_cryptopp.h"
#include "src/hash/php_hash_transformation_interface.h"
#include "php_mac_interface.h"

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_MacInterface;

static zend_function_entry cryptopp_methods_MacInterface[] = {
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, setKey, arginfo_MacInterface_setKey)
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, getKey, arginfo_MacInterface_getKey)
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, isValidKeyLength, arginfo_MacInterface_isValidKeyLength)
    PHP_FE_END
};

void init_interface_MacInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "MacInterface", cryptopp_methods_MacInterface);
    cryptopp_ce_MacInterface = zend_register_internal_interface(&ce TSRMLS_CC);

    zend_class_implements(cryptopp_ce_MacInterface TSRMLS_CC, 1, cryptopp_ce_HashTransformationInterface);
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
