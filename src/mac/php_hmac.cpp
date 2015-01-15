#include "src/php_cryptopp.h"
#include "src/hash/php_hash_interface.h"
#include "src/hash/php_hash_abstract.h"
#include "src/hash/hash_proxy.h"
#include "src/utils/zval_utils.h"
#include "php_mac_abstract.h"
#include "php_hmac.h"
#include <hmac.h>
#include <string>
#include <zend_exceptions.h>

/* {{{ fork of CryptoPP::HMAC that take a hash as parameter instead of a template parameter */
Hmac::Hmac(CryptoPP::HashTransformation *hash, bool freeHashObject)
{
    m_hash              = hash;
    m_freeHashObject    = freeHashObject;
}

Hmac::~Hmac()
{
    if (m_freeHashObject) {
        delete m_hash;
    }
}
/* }}} */

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_MacHmac_construct, 0)
    ZEND_ARG_OBJ_INFO(0, hashAlgo, Cryptopp\\HashInterface, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_MacHmac;

static zend_function_entry cryptopp_methods_MacHmac[] = {
    PHP_ME(Cryptopp_MacHmac, __construct, arginfo_MacHmac_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

void init_class_MacHmac(TSRMLS_D) {
    init_class_MacAbstractChild("hmac", "MacHmac", &cryptopp_ce_MacHmac, cryptopp_methods_MacHmac TSRMLS_CC);
    zend_declare_property_null(cryptopp_ce_MacHmac, "hash", 4,  ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ proto MacHmac::__construct(Cryptopp\HashInterface hashAlgo) */
PHP_METHOD(Cryptopp_MacHmac, __construct) {
    zval *hashObject;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &hashObject, cryptopp_ce_HashInterface)) {
        return;
    }

    CryptoPP::MessageAuthenticationCode *mac;
    CryptoPP::HashTransformation *hash;

    if (instanceof_function(Z_OBJCE_P(hashObject), cryptopp_ce_HashAbstract TSRMLS_CC)) {
        // retrieve native hash object
        hash    = getCryptoppHashNativePtr(hashObject TSRMLS_CC);
        mac     = new Hmac(hash, false);
    } else {
        // create a proxy to the user php object
        hash    = new HashProxy(hashObject TSRMLS_CC);
        mac     = new Hmac(hash, true);
    }

    // ensure that the hash algorithm is compatible
    if (0 == hash->BlockSize()) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s can only be used with a block-based hash function (block size > 0)", cryptopp_ce_MacHmac->name);
        delete mac;
        return;
    } else if (hash->BlockSize() < hash->DigestSize()) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s: hash block size (%d) cannot be lower than digest size (%d)", cryptopp_ce_MacHmac->name, hash->BlockSize(), hash->DigestSize());
        delete mac;
        return;
    }

    setCryptoppMacNativePtr(getThis(), mac  TSRMLS_CC);

    // compute algo name
    zval *funcName      = makeZval("getName");
    zval *hashAlgoName  = call_user_method(hashObject, funcName TSRMLS_CC);

    std::string name("hmac(");
    name.append(Z_STRVAL_P(hashAlgoName), Z_STRLEN_P(hashAlgoName));
    name.append(")");
    zend_update_property_stringl(cryptopp_ce_MacAbstract, getThis(), "name", 4, name.c_str(), name.length() TSRMLS_CC);

    zval_ptr_dtor(&hashAlgoName);
    zval_ptr_dtor(&funcName);

    // set a default empty key
    byte defaultKey[0];
    mac->SetKey(defaultKey, 0);

    // hold the hash object. if not, it can be deleted.
    zend_update_property(cryptopp_ce_MacHmac, getThis(), "hash", 4, hashObject TSRMLS_CC);
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
