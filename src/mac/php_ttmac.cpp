#include "../php_cryptopp.h"
#include "php_mac_interface.h"
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
    CRYPTOPP_MAC_REQUIRED_METHODS(MacTwoTrackMac)
    PHP_FE_END
};

CRYPTOPP_MAC_INIT_CLASS("two_track_mac", MacTwoTrackMac, cryptopp_ce_MacTwoTrackMac, cryptopp_methods_MacTwoTrackMac)
/* }}} */

/* {{{ proto MacTwoTrackMac::__construct(void) */
PHP_METHOD(Cryptopp_MacTwoTrackMac, __construct) {
    CryptoPP::TTMAC *mac;
    mac = new CryptoPP::TTMAC();
    CRYPTOPP_MAC_SET_NATIVE_PTR(mac)
}
/* }}} */

/* {{{ proto string MacTwoTrackMac::getName(void)
   Return algorithm name */
PHP_METHOD(Cryptopp_MacTwoTrackMac, getName) {
    RETURN_STRING("two_track_mac", 1);
}
/* }}} */

/* include common hash methods definitions */
CRYPTOPP_MAC_COMMON_METHODS_DEFINITIONS(MacTwoTrackMac)

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
