#include "../php_cryptopp.h"
#include "php_mac_interface.h"
#include "php_hmac.h"
#include "php_hmac_d.h"
#include <hmac.h>
#include <string>
#include <zend_exceptions.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_MacHmac_construct, 0)
    ZEND_ARG_INFO(0, hashAlgo)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_MacHmac;

static zend_function_entry cryptopp_methods_MacHmac[] = {
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_MacHmac, __construct, arginfo_MacHmac_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    CRYPTOPP_MAC_REQUIRED_METHODS(MacHmac)
    PHP_FE_END
};

CRYPTOPP_MAC_INIT_CLASS(MacHmac, cryptopp_ce_MacHmac, cryptopp_methods_MacHmac)

void init_class_MacHmac_(TSRMLS_D) {
    CRYPTOPP_MAC_INIT_CLASS_FUNC_CALL(MacHmac)
    zend_declare_property_string(cryptopp_ce_MacHmac, "hashAlgo", 8, "",  ZEND_ACC_PROTECTED TSRMLS_CC);
}
/* }}} */

/* {{{ proto MacHmac::__construct(string hashAlgo) */
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_MacHmac, __construct) {
    char *hashAlgo      = NULL;
    int hashAlgoSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &hashAlgo, &hashAlgoSize)) {
        return;
    }

    // pick the right HMAC implementation
    std::string algoName(hashAlgo, hashAlgoSize);
    CryptoPP::MessageAuthenticationCode *mac;

    CRYPTOPP_MAC_HMAC_CONDITIONAL_CREATION(algoName, mac)
    else {
        // not a valid hash algo name
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : unknown hash algorithm %s", cryptopp_ce_MacHmac->name, algoName.c_str());
        RETURN_NULL();
    }

    CRYPTOPP_MAC_SET_NATIVE_PTR(mac)
    zend_update_property_stringl(cryptopp_ce_MacHmac, getThis(), "hashAlgo", 8, hashAlgo, hashAlgoSize TSRMLS_CC);

    // set a default empty key
    byte defaultKey[0];
    mac->SetKey(defaultKey, 0);
}
/* }}} */

/* {{{ proto string MacHmac::getName(void)
   Return algorithm name */
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_MacHmac, getName) {
    zval *hashAlgo;
    hashAlgo = zend_read_property(cryptopp_ce_MacHmac, getThis(), "hashAlgo", 8, 0 TSRMLS_CC);

    std::string name("hmac(");
    name.append(Z_STRVAL_P(hashAlgo), Z_STRLEN_P(hashAlgo));
    name.append(")");

    RETURN_STRING(name.c_str(), 1);
}
/* }}} */

/* include common hash methods definitions */
CRYPTOPP_MAC_COMMON_METHODS_DEFINITIONS(MacHmac)
