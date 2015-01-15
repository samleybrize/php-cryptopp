#include "src/php_cryptopp.h"
#include "php_mac_abstract.h"
#include "php_ttmac.h"
#include <ttmac.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_MacTwoTrackMac_construct, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_MacTwoTrackMac;

static zend_function_entry cryptopp_methods_MacTwoTrackMac[] = {
    PHP_ME(Cryptopp_MacTwoTrackMac, __construct, arginfo_MacTwoTrackMac_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

void init_class_MacTwoTrackMac(TSRMLS_D) {
    init_class_MacAbstractChild("two_track_mac", "MacTwoTrackMac", &cryptopp_ce_MacTwoTrackMac, cryptopp_methods_MacTwoTrackMac TSRMLS_CC);
}
/* }}} */

/* {{{ proto MacTwoTrackMac::__construct(void) */
PHP_METHOD(Cryptopp_MacTwoTrackMac, __construct) {
    CryptoPP::TTMAC *mac = new CryptoPP::TTMAC();
    setCryptoppMacNativePtr(getThis(), mac TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_MacAbstract, getThis(), "name", 4, "two_track_mac", 13 TSRMLS_CC);
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
