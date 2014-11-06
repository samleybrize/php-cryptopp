#include "../php_cryptopp.h"
#include "php_rbg_interface.h"
#include "php_rbg.h"
#include <osrng.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_RandomByteGenerator_construct, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_RandomByteGenerator;

static zend_function_entry cryptopp_methods_RandomByteGenerator[] = {
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_RandomByteGenerator, __construct, arginfo_RandomByteGenerator_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    CRYPTOPP_RBG_REQUIRED_METHODS(RandomByteGenerator)
    PHP_FE_END
};

CRYPTOPP_RBG_INIT_CLASS(RandomByteGenerator, cryptopp_ce_RandomByteGenerator, cryptopp_methods_RandomByteGenerator)
/* }}} */

/* {{{ proto RandomByteGenerator::__construct(void) */
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_RandomByteGenerator, __construct) {
    CryptoPP::AutoSeededRandomPool *rbg;
    rbg = new CryptoPP::AutoSeededRandomPool();
    CRYPTOPP_RBG_SET_NATIVE_PTR(rbg)
}
/* }}} */

/* include common rbg methods definitions */
CRYPTOPP_RBG_COMMON_METHODS_DEFINITIONS(RandomByteGenerator, CryptoPP::AutoSeededRandomPool)
