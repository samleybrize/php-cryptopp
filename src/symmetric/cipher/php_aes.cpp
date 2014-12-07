#include "../../php_cryptopp.h"
#include "php_aes.h"
#include "php_block_cipher_abstract.h"
#include <aes.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_SymmetricCipherAes_construct, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_SymmetricCipherAes;

static zend_function_entry cryptopp_methods_SymmetricCipherAes[] = {
    PHP_ME(Cryptopp_SymmetricCipherAes, __construct, arginfo_SymmetricCipherAes_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

void init_class_SymmetricCipherAes(TSRMLS_D) {
    init_class_BlockCipherAbstractChild("aes", "SymmetricCipherAes", cryptopp_ce_SymmetricCipherAes, cryptopp_methods_SymmetricCipherAes TSRMLS_CC);
}
/* }}} */

/* {{{ proto SymmetricCipherAes::__construct(void) */
PHP_METHOD(Cryptopp_SymmetricCipherAes, __construct) {
    CryptoPP::AES::Encryption *encryptor;
    CryptoPP::AES::Decryption *decryptor;
    encryptor = new CryptoPP::AES::Encryption();
    decryptor = new CryptoPP::AES::Decryption();
    setCryptoppBlockCipherEncryptorPtr(getThis(), encryptor TSRMLS_CC);
    setCryptoppBlockCipherDecryptorPtr(getThis(), decryptor TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_BlockCipherAbstract, getThis(), "name", 4, "aes", 3 TSRMLS_CC);
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
