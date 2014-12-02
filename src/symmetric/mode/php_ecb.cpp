#include "../../php_cryptopp.h"
#include "../cipher/php_symmetric_cipher_abstract.h"
#include "php_symmetric_mode_interface.h"
#include "php_ecb.h"
#include <modes.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_SymmetricModeEcb_construct, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_SymmetricModeEcb;

static zend_function_entry cryptopp_methods_SymmetricModeEcb[] = {
    PHP_ME(Cryptopp_SymmetricModeEcb, __construct, arginfo_SymmetricModeEcb_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    CRYPTOPP_SYMMETRIC_MODE_REQUIRED_METHODS(SymmetricModeEcb)
    PHP_FE_END
};

CRYPTOPP_SYMMETRIC_MODE_INIT_CLASS("ecb", SymmetricModeEcb, cryptopp_ce_SymmetricModeEcb, cryptopp_methods_SymmetricModeEcb)
/* }}} */

/* {{{ proto SymmetricModeEcb::__construct(void) */
PHP_METHOD(Cryptopp_SymmetricModeEcb, __construct) {
    zval *cipherObject;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &cipherObject, cryptopp_ce_SymmetricCipherAbstract)) {
        return;
    }

    // get needed cipher elements
    CryptoPP::BlockCipher *cipherEncryptor;
    CryptoPP::BlockCipher *cipherDecryptor;
    std::string *cipherName;

    if (!cryptoppSymmetricModeGetCipherElements(cipherObject, getThis(), &cipherEncryptor, &cipherDecryptor, &cipherName)) {
        RETURN_NULL()
    }

    // instanciate mode encryptor/decryptor
    CryptoPP::ECB_Mode_ExternalCipher::Encryption *encryptor;
    CryptoPP::ECB_Mode_ExternalCipher::Decryption *decryptor;
    encryptor = new CryptoPP::ECB_Mode_ExternalCipher::Encryption(*cipherEncryptor);
    decryptor = new CryptoPP::ECB_Mode_ExternalCipher::Decryption(*cipherDecryptor);
    CRYPTOPP_SYMMETRIC_MODE_SET_ENCRYPTOR_PTR(encryptor)
    CRYPTOPP_SYMMETRIC_MODE_SET_DECRYPTOR_PTR(decryptor)

    std::string name("ecb(");
    name.append(*cipherName);
    name.append(")");
    zend_update_property_stringl(cryptopp_ce_SymmetricModeEcb, getThis(), "name", 4, name.c_str(), name.size() TSRMLS_CC);

    // hold the cipher object. if not, it can be deleted and associated encryptor/decryptor objects will be deleted too
    zend_update_property(cryptopp_ce_SymmetricModeEcb, getThis(), "cipher", 6, cipherObject TSRMLS_CC);
}
/* }}} */

/* {{{ proto string SymmetricModeEcb::getName(void)
   Return algorithm name */
PHP_METHOD(Cryptopp_SymmetricModeEcb, getName) {
    zval *name;
    name = zend_read_property(cryptopp_ce_SymmetricModeEcb, getThis(), "name", 4, 0 TSRMLS_CC);
    RETURN_ZVAL(name, 1, 0);
}
/* }}} */

/* include common hash methods definitions */
CRYPTOPP_SYMMETRIC_MODE_COMMON_METHODS_DEFINITIONS(SymmetricModeEcb, CryptoPP::Weak::MD5)

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
