#include "../../../php_cryptopp.h"
#include "php_stream_cipher_abstract.h"
#include "php_sosemanuk.h"
#include <sosemanuk.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_StreamCipherSosemanuk_construct, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_StreamCipherSosemanuk;

static zend_function_entry cryptopp_methods_StreamCipherSosemanuk[] = {
    PHP_ME(Cryptopp_StreamCipherSosemanuk, __construct, arginfo_StreamCipherSosemanuk_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

void init_class_StreamCipherSosemanuk(TSRMLS_D) {
    init_class_StreamCipherAbstractChild("sosemanuk", "StreamCipherSosemanuk", &cryptopp_ce_StreamCipherSosemanuk, cryptopp_methods_StreamCipherSosemanuk TSRMLS_CC);
}
/* }}} */

/* {{{ proto StreamCipherSosemanuk::__construct(void) */
PHP_METHOD(Cryptopp_StreamCipherSosemanuk, __construct) {
    CryptoPP::Sosemanuk::Encryption *encryptor;
    CryptoPP::Sosemanuk::Decryption *decryptor;
    encryptor = new CryptoPP::Sosemanuk::Encryption();
    decryptor = new CryptoPP::Sosemanuk::Decryption();
    setCryptoppStreamCipherEncryptorPtr(getThis(), encryptor TSRMLS_CC);
    setCryptoppStreamCipherDecryptorPtr(getThis(), decryptor TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_StreamCipherAbstract, getThis(), "name", 4, "sosemanuk", 9 TSRMLS_CC);
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
