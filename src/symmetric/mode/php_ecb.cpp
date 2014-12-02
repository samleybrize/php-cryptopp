#include "../../php_cryptopp.h"
#include "../cipher/php_symmetric_cipher_abstract.h"
#include "php_symmetric_mode_abstract.h"
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
    PHP_FE_END
};

void init_class_SymmetricModeEcb(TSRMLS_D) {
    init_class_SymmetricModeAbstractChild("ecb", "SymmetricModeEcb", cryptopp_ce_SymmetricModeEcb, cryptopp_methods_SymmetricModeEcb TSRMLS_CC);
}
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
    setCryptoppSymmetricModeEncryptorPtr(getThis(), encryptor TSRMLS_CC);
    setCryptoppSymmetricModeDecryptorPtr(getThis(), decryptor TSRMLS_CC);

    std::string name("ecb(");
    name.append(*cipherName);
    name.append(")");
    zend_update_property_stringl(cryptopp_ce_SymmetricModeAbstract, getThis(), "name", 4, name.c_str(), name.size() TSRMLS_CC);

    // hold the cipher object. if not, it can be deleted and associated encryptor/decryptor objects will be deleted too
    zend_update_property(cryptopp_ce_SymmetricModeAbstract, getThis(), "cipher", 6, cipherObject TSRMLS_CC);
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
