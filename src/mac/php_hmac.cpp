#include "../php_cryptopp.h"
#include "php_mac_abstract.h"
#include "php_hmac.h"
#include "php_hmac_d.h"
#include "../hash/php_hash_abstract.h"
#include <hmac.h>
#include <string>
#include <zend_exceptions.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_MacHmac_construct, 0)
    ZEND_ARG_OBJ_INFO(0, hashAlgo, Cryptopp\\HashAbstract, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_MacHmac;

static zend_function_entry cryptopp_methods_MacHmac[] = {
    PHP_ME(Cryptopp_MacHmac, __construct, arginfo_MacHmac_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

void init_class_MacHmac(TSRMLS_D) {
    init_class_MacAbstractChild("hmac", "MacHmac", cryptopp_ce_MacHmac, cryptopp_methods_MacHmac TSRMLS_CC);
}
/* }}} */

/* {{{ proto MacHmac::__construct(Cryptopp\HashAbstract hashAlgo) */
PHP_METHOD(Cryptopp_MacHmac, __construct) {
    zval *hashObject;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &hashObject, cryptopp_ce_HashAbstract)) {
        return;
    }

    // retrieve the name of the hash algorithm
    zval *hashAlgoName;
    zval *funcName;
    MAKE_STD_ZVAL(hashAlgoName);
    MAKE_STD_ZVAL(funcName);
    ZVAL_STRING(funcName, "getName", 1);
    call_user_function(NULL, &hashObject, funcName, hashAlgoName, 0, NULL TSRMLS_CC);

    // pick the right HMAC implementation
    std::string algoName(Z_STRVAL_P(hashAlgoName), Z_STRLEN_P(hashAlgoName));
    CryptoPP::MessageAuthenticationCode *mac;
    zval_dtor(hashAlgoName);
    zval_dtor(funcName);

    CRYPTOPP_MAC_HMAC_CONDITIONAL_CREATION(algoName, mac)
    else {
        // not a valid hash algo name
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : unsupported hash algorithm %s", cryptopp_ce_MacHmac->name, algoName.c_str());
        RETURN_NULL();
    }

    setCryptoppMacNativePtr(getThis(), mac  TSRMLS_CC);

    std::string name("hmac(");
    name.append(algoName);
    name.append(")");
    zend_update_property_stringl(cryptopp_ce_MacAbstract, getThis(), "name", 4, name.c_str(), name.length() TSRMLS_CC);

    // set a default empty key
    byte defaultKey[0];
    mac->SetKey(defaultKey, 0);
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
