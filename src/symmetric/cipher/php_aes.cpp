#include "../../php_cryptopp.h"
#include "php_symmetric_cipher_interface.h"
#include "php_aes.h"
#include <aes.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_SymmetricCipherAes_construct, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_SymmetricCipherAes;

static zend_function_entry cryptopp_methods_SymmetricCipherAes[] = {
    PHP_ME(Cryptopp_SymmetricCipherAes, __construct, arginfo_SymmetricCipherAes_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    CRYPTOPP_SYMMETRIC_CIPHER_REQUIRED_METHODS(SymmetricCipherAes)
    PHP_FE_END
};

CRYPTOPP_SYMMETRIC_CIPHER_INIT_CLASS("aes", SymmetricCipherAes, cryptopp_ce_SymmetricCipherAes, cryptopp_methods_SymmetricCipherAes)
/* }}} */

/* {{{ proto SymmetricCipherAes::__construct(void) */
PHP_METHOD(Cryptopp_SymmetricCipherAes, __construct) {
    CryptoPP::AES::Encryption *encryptor;
    CryptoPP::AES::Decryption *decryptor;
    encryptor = new CryptoPP::AES::Encryption();
    decryptor = new CryptoPP::AES::Decryption();
    CRYPTOPP_SYMMETRIC_CIPHER_SET_ENCRYPTOR_PTR(encryptor)
    CRYPTOPP_SYMMETRIC_CIPHER_SET_DECRYPTOR_PTR(decryptor)

    zend_update_property_stringl(cryptopp_ce_SymmetricCipherAes, getThis(), "name", 4, "aes", 3 TSRMLS_CC);
}
/* }}} */

/* {{{ proto string SymmetricCipherAes::getName(void)
   Return algorithm name */
PHP_METHOD(Cryptopp_SymmetricCipherAes, getName) {
    RETURN_STRING("aes", 1);
}
/* }}} */

/* include common hash methods definitions */
CRYPTOPP_SYMMETRIC_CIPHER_COMMON_METHODS_DEFINITIONS(SymmetricCipherAes, CryptoPP::Weak::MD5)

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
