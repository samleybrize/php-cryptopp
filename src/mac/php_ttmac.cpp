#include "../php_cryptopp.h"
#include "php_mac_interface.h"
#include "php_ttmac.h"
#include <ttmac.h>

// TODO key
// TODO = key in constructor (optional)
// TODO = setKey() method (check key length)

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_MacTwoTrackMac_construct, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_MacTwoTrackMac;

static zend_function_entry cryptopp_methods_MacTwoTrackMac[] = {
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_MacTwoTrackMac, __construct, arginfo_MacTwoTrackMac_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    CRYPTOPP_MAC_REQUIRED_METHODS(MacTwoTrackMac)
    PHP_FE_END
};

CRYPTOPP_MAC_INIT_CLASS(MacTwoTrackMac, cryptopp_ce_MacTwoTrackMac, cryptopp_methods_MacTwoTrackMac)
/* }}} */

/* {{{ proto MacTwoTrackMac::__construct(void) */
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_MacTwoTrackMac, __construct) {
    CryptoPP::TTMAC *mac;
    mac = new CryptoPP::TTMAC();
    CRYPTOPP_MAC_SET_NATIVE_PTR(MacTwoTrackMac, mac)
}
/* }}} */

/* {{{ proto string MacTwoTrackMac::getName(void)
   Return algorithm name */
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_MacTwoTrackMac, getName) {
    RETURN_STRING("two_track_mac", 1);
}
/* }}} */

/* include common hash methods definitions */
CRYPTOPP_MAC_COMMON_METHODS_DEFINITIONS(MacTwoTrackMac, CryptoPP::TTMAC)
