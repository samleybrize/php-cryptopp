/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "src/php_cryptopp.h"
#include "php_block_cipher_abstract.h"
#include "php_aes.h"
#include <aes.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_BlockCipherAes_construct, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_BlockCipherAes;

static zend_function_entry cryptopp_methods_BlockCipherAes[] = {
    PHP_ME(Cryptopp_BlockCipherAes, __construct, arginfo_BlockCipherAes_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

void init_class_BlockCipherAes(TSRMLS_D) {
    init_class_BlockCipherAbstractChild("aes", "BlockCipherAes", &cryptopp_ce_BlockCipherAes, cryptopp_methods_BlockCipherAes TSRMLS_CC);
}
/* }}} */

/* {{{ proto BlockCipherAes::__construct(void) */
PHP_METHOD(Cryptopp_BlockCipherAes, __construct) {
    CryptoPP::AES::Encryption *encryptor = new CryptoPP::AES::Encryption();
    CryptoPP::AES::Decryption *decryptor = new CryptoPP::AES::Decryption();
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
