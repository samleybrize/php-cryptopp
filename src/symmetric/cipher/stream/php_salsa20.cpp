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
#include "src/utils/zval_utils.h"
#include "php_stream_cipher_abstract.h"
#include "php_salsa20.h"
#include <salsa.h>
#include <zend_exceptions.h>

/* {{{ fork of CryptoPP::Salsa20 that allow to set the number of rounds */
Salsa20::Base::Base(zval *object TSRMLS_DC) : m_object(object)
{
   SET_M_TSRMLS_C()
}

Salsa20::Encryption::Encryption(zval *object TSRMLS_DC)
    : Base(object TSRMLS_CC)
    , CryptoPP::Salsa20::Encryption()
{
    m_rounds = 20;
}

Salsa20::Decryption::Decryption(zval *object TSRMLS_DC)
    : Base(object TSRMLS_CC)
    , CryptoPP::Salsa20::Decryption()
{
    m_rounds = 20;
}

void Salsa20::Encryption::SetRounds(int rounds)
{
    m_rounds = rounds;

    // restart
    zval *funcname  = makeZval("restart");
    zval *output    = call_user_method(m_object, funcname M_TSRMLS_CC);
    zval_ptr_dtor(&funcname);
    zval_ptr_dtor(&output);
}

void Salsa20::Decryption::SetRounds(int rounds)
{
    m_rounds = rounds;

    // restart
    zval *funcname  = makeZval("restart");
    zval *output    = call_user_method(m_object, funcname M_TSRMLS_CC);
    zval_ptr_dtor(&funcname);
    zval_ptr_dtor(&output);
}

void Salsa20::Encryption::CipherSetKey(const CryptoPP::NameValuePairs &params, const byte *key, size_t length)
{
    int rounds = m_rounds;
    CryptoPP::Salsa20::Encryption::CipherSetKey(params, key, length);

    if (rounds > 0) {
        m_rounds = rounds;
    }
}

void Salsa20::Decryption::CipherSetKey(const CryptoPP::NameValuePairs &params, const byte *key, size_t length)
{
    int rounds = m_rounds;
    CryptoPP::Salsa20::Decryption::CipherSetKey(params, key, length);

    if (rounds > 0) {
        m_rounds = rounds;
    }
}
/* }}} */

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_StreamCipherSalsa20_construct, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_StreamCipherSalsa20_setRounds, 0)
    ZEND_ARG_INFO(0, rounds)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_StreamCipherSalsa20;

static zend_function_entry cryptopp_methods_StreamCipherSalsa20[] = {
    PHP_ME(Cryptopp_StreamCipherSalsa20, __construct, arginfo_StreamCipherSalsa20_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Cryptopp_StreamCipherSalsa20, setRounds, arginfo_StreamCipherSalsa20_setRounds, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_StreamCipherSalsa20(TSRMLS_D) {
    init_class_StreamCipherAbstractChild("salsa20", "StreamCipherSalsa20", &cryptopp_ce_StreamCipherSalsa20, cryptopp_methods_StreamCipherSalsa20 TSRMLS_CC);
}
/* }}} */

/* {{{ proto StreamCipherSalsa20::__construct(void) */
PHP_METHOD(Cryptopp_StreamCipherSalsa20, __construct) {
    Salsa20::Encryption *encryptor = new Salsa20::Encryption(getThis() TSRMLS_CC);
    Salsa20::Decryption *decryptor = new Salsa20::Decryption(getThis() TSRMLS_CC);
    setCryptoppStreamCipherEncryptorPtr(getThis(), encryptor TSRMLS_CC);
    setCryptoppStreamCipherDecryptorPtr(getThis(), decryptor TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_StreamCipherAbstract, getThis(), "name", 4, "salsa20", 7 TSRMLS_CC);
}
/* }}} */

/* {{{ proto void StreamCipherSalsa20::setRounds(int)
   Sets the number of rounds */
PHP_METHOD(Cryptopp_StreamCipherSalsa20, setRounds) {
    long rounds;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &rounds)) {
        return;
    }

    // check rounds
    if (8 != rounds && 12 != rounds && 20 != rounds) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\StreamCipherSalsa20: number of rounds must be one of 8, 12 or 20");
        return;
    }

    // set rounds
    CryptoPP::SymmetricCipher *encryptor;
    CryptoPP::SymmetricCipher *decryptor;
    CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)
    CRYPTOPP_STREAM_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor)

    dynamic_cast<Salsa20::Encryption*>(encryptor)->SetRounds(rounds);
    dynamic_cast<Salsa20::Decryption*>(decryptor)->SetRounds(rounds);
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
