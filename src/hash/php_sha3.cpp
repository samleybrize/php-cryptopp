#include "src/php_cryptopp.h"
#include "php_hash_abstract.h"
#include "php_sha3.h"
#include <sha3.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_HashSha3_224_construct, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashSha3_256_construct, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashSha3_384_construct, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashSha3_512_construct, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP classes déclaration */
zend_class_entry *cryptopp_ce_HashSha3_224;
zend_class_entry *cryptopp_ce_HashSha3_256;
zend_class_entry *cryptopp_ce_HashSha3_384;
zend_class_entry *cryptopp_ce_HashSha3_512;

static zend_function_entry cryptopp_methods_HashSha3_224[] = {
    PHP_ME(Cryptopp_HashSha3_224, __construct, arginfo_HashSha3_224_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

static zend_function_entry cryptopp_methods_HashSha3_256[] = {
    PHP_ME(Cryptopp_HashSha3_256, __construct, arginfo_HashSha3_256_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

static zend_function_entry cryptopp_methods_HashSha3_384[] = {
    PHP_ME(Cryptopp_HashSha3_384, __construct, arginfo_HashSha3_384_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

static zend_function_entry cryptopp_methods_HashSha3_512[] = {
    PHP_ME(Cryptopp_HashSha3_512, __construct, arginfo_HashSha3_512_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

void init_classes_HashSha3(TSRMLS_D)
{
    init_class_HashAbstractChild("sha3_224", "HashSha3_224", &cryptopp_ce_HashSha3_224, cryptopp_methods_HashSha3_224 TSRMLS_CC);
    init_class_HashAbstractChild("sha3_256", "HashSha3_256", &cryptopp_ce_HashSha3_256, cryptopp_methods_HashSha3_256 TSRMLS_CC);
    init_class_HashAbstractChild("sha3_384", "HashSha3_384", &cryptopp_ce_HashSha3_384, cryptopp_methods_HashSha3_384 TSRMLS_CC);
    init_class_HashAbstractChild("sha3_512", "HashSha3_512", &cryptopp_ce_HashSha3_512, cryptopp_methods_HashSha3_512 TSRMLS_CC);
}
/* }}} */

/* {{{ proto HashSha3_*::__construct(void) */
PHP_METHOD(Cryptopp_HashSha3_224, __construct) {
    SHA3_224 *hash = new SHA3_224();
    setCryptoppHashNativePtr(getThis(), hash TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_HashAbstract, getThis(), "name", 4, "sha3_224", 8 TSRMLS_CC);
}

PHP_METHOD(Cryptopp_HashSha3_256, __construct) {
    SHA3_256 *hash = new SHA3_256();
    setCryptoppHashNativePtr(getThis(), hash TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_HashAbstract, getThis(), "name", 4, "sha3_256", 8 TSRMLS_CC);
}

PHP_METHOD(Cryptopp_HashSha3_384, __construct) {
    SHA3_384 *hash = new SHA3_384();
    setCryptoppHashNativePtr(getThis(), hash TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_HashAbstract, getThis(), "name", 4, "sha3_384", 8 TSRMLS_CC);
}

PHP_METHOD(Cryptopp_HashSha3_512, __construct) {
    SHA3_512 *hash = new SHA3_512();
    setCryptoppHashNativePtr(getThis(), hash TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_HashAbstract, getThis(), "name", 4, "sha3_512", 8 TSRMLS_CC);
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
