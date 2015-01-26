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
#include "src/symmetric/cipher/block/php_block_cipher_interface.h"
#include "php_authenticated_symmetric_cipher_abstract.h"
#include "php_eax.h"
#include <zend_exceptions.h>
#include <eax.h>

/* {{{ fork of CryptoPP::EAX that take a cipher as parameter instead of a template parameter */
EAX::Base::Base(CryptoPP::BlockCipher *cipher, bool cipherMustBeDestructed)
    : m_cipher(cipher)
    , m_cipherMustBeDestructed(cipherMustBeDestructed)
    , m_cmac(cipher)
{
}

EAX::Base::~Base()
{
    if (m_cipherMustBeDestructed) {
        delete m_cipher;
    }
}
/* }}} */

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherEax_construct, 0)
    ZEND_ARG_OBJ_INFO(0, cipher, Cryptopp\\BlockCipherInterface, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_AuthenticatedSymmetricCipherEax;

static zend_function_entry cryptopp_methods_AuthenticatedSymmetricCipherEax[] = {
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherEax, __construct, arginfo_AuthenticatedSymmetricCipherEax_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

void init_class_AuthenticatedSymmetricCipherEax(TSRMLS_D) {
    init_class_AuthenticatedSymmetricCipherAbstractChild("eax", "AuthenticatedSymmetricCipherEax", &cryptopp_ce_AuthenticatedSymmetricCipherEax, cryptopp_methods_AuthenticatedSymmetricCipherEax TSRMLS_CC);
}
/* }}} */

/* {{{ proto AuthenticatedSymmetricCipherEax::__construct(Cryptopp\BlockCipherInterface cipher) */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherEax, __construct) {
    zval *cipherObject;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &cipherObject, cryptopp_ce_BlockCipherInterface)) {
        return;
    }

    // get needed cipher elements
    CryptoPP::BlockCipher *cipherEncryptor;
    CryptoPP::BlockCipher *cipherDecryptor;
    std::string *authenticatedCipherName;
    bool cipherMustBeDestructed;

    if (!cryptoppAuthenticatedSymmetricCipherGetCipherElements("eax", cipherObject, getThis(), &cipherEncryptor, &cipherDecryptor, &authenticatedCipherName, cipherMustBeDestructed TSRMLS_CC)) {
        RETURN_NULL()
    }

    if (cipherMustBeDestructed) {
        delete cipherDecryptor;
    }

    // instanciate authenticated cipher encryptor/decryptor
    EAX::Encryption *encryptor = new EAX::Encryption(cipherEncryptor, cipherMustBeDestructed);
    EAX::Decryption *decryptor = new EAX::Decryption(cipherEncryptor, false);
    setCryptoppAuthenticatedSymmetricCipherEncryptorPtr(getThis(), encryptor TSRMLS_CC);
    setCryptoppAuthenticatedSymmetricCipherDecryptorPtr(getThis(), decryptor TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, getThis(), "name", 4, authenticatedCipherName->c_str(), authenticatedCipherName->length() TSRMLS_CC);
    delete authenticatedCipherName;

    // hold the cipher object. if not, it can be deleted and associated encryptor/decryptor objects will be deleted too
    zend_update_property(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, getThis(), "cipher", 6, cipherObject TSRMLS_CC);
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
