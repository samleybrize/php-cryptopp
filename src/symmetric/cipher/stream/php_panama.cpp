/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "src/php_cryptopp.h"
#include "src/exception/php_exception.h"
#include "php_stream_cipher_abstract.h"
#include "php_panama.h"
#include <panama.h>
#include <zend_exceptions.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(arginfo_StreamCipherPanama_construct, 0, 0, 0)
    ZEND_ARG_INFO(0, endianness)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_StreamCipherPanama;

static zend_function_entry cryptopp_methods_StreamCipherPanama[] = {
    PHP_ME(Cryptopp_StreamCipherPanama, __construct, arginfo_StreamCipherPanama_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

void init_class_StreamCipherPanama(TSRMLS_D) {
    init_class_StreamCipherAbstractChild("panama", "StreamCipherPanama", &cryptopp_ce_StreamCipherPanama, cryptopp_methods_StreamCipherPanama TSRMLS_CC);
}
/* }}} */

/* {{{ proto StreamCipherPanama::__construct(void) */
PHP_METHOD(Cryptopp_StreamCipherPanama, __construct) {
    long endianness = PHP_CRYPTOPP_LITTLE_ENDIAN;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &endianness)) {
        return;
    }

    CryptoPP::SymmetricCipher *encryptor;
    CryptoPP::SymmetricCipher *decryptor;

    if (PHP_CRYPTOPP_LITTLE_ENDIAN == endianness) {
        // little endian
        encryptor = new CryptoPP::PanamaCipher<CryptoPP::LittleEndian>::Encryption();
        decryptor = new CryptoPP::PanamaCipher<CryptoPP::LittleEndian>::Decryption();
    } else if (PHP_CRYPTOPP_BIG_ENDIAN == endianness) {
        // big endian
        encryptor = new CryptoPP::PanamaCipher<CryptoPP::BigEndian>::Encryption();
        decryptor = new CryptoPP::PanamaCipher<CryptoPP::BigEndian>::Decryption();
    } else {
        // invalid endianness
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\StreamCipherPanama: invalid endianness. Must be either Cryptopp\\LITTLE_ENDIAN or Cryptopp\\BIG_ENDIAN.");
        return;
    }

    setCryptoppStreamCipherEncryptorPtr(getThis(), encryptor TSRMLS_CC);
    setCryptoppStreamCipherDecryptorPtr(getThis(), decryptor TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_StreamCipherAbstract, getThis(), "name", 4, "panama", 6 TSRMLS_CC);
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
