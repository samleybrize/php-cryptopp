/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "src/php_cryptopp.h"
#include "php_stream_cipher_abstract.h"
#include "php_salsa20.h"
#include <salsa.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_StreamCipherSalsa20_construct, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_StreamCipherSalsa20;

static zend_function_entry cryptopp_methods_StreamCipherSalsa20[] = {
    PHP_ME(Cryptopp_StreamCipherSalsa20, __construct, arginfo_StreamCipherSalsa20_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

void init_class_StreamCipherSalsa20(TSRMLS_D) {
    init_class_StreamCipherAbstractChild("salsa20", "StreamCipherSalsa20", &cryptopp_ce_StreamCipherSalsa20, cryptopp_methods_StreamCipherSalsa20 TSRMLS_CC);
}
/* }}} */

/* {{{ proto StreamCipherSalsa20::__construct(void) */
PHP_METHOD(Cryptopp_StreamCipherSalsa20, __construct) {
    CryptoPP::Salsa20::Encryption *encryptor = new CryptoPP::Salsa20::Encryption();
    CryptoPP::Salsa20::Decryption *decryptor = new CryptoPP::Salsa20::Decryption();
    setCryptoppStreamCipherEncryptorPtr(getThis(), encryptor TSRMLS_CC);
    setCryptoppStreamCipherDecryptorPtr(getThis(), decryptor TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_StreamCipherAbstract, getThis(), "name", 4, "salsa20", 7 TSRMLS_CC);
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
