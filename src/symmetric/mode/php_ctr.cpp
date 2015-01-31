/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "src/php_cryptopp.h"
#include "src/symmetric/cipher/block/php_block_cipher_interface.h"
#include "php_symmetric_mode_abstract.h"
#include "php_ctr.h"
#include <modes.h>

/* {{{ fork of CryptoPP::CTR_Mode_ExternalCipher to support block cipher destruction */
Ctr::Base::~Base()
{
    if (m_cipherMustBeDestructed) {
        delete m_cipher;
    }
}
/* }}} */

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_SymmetricModeCtr_construct, 0)
    ZEND_ARG_OBJ_INFO(0, cipher, Cryptopp\\BlockCipherInterface, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_SymmetricModeCtr;

static zend_function_entry cryptopp_methods_SymmetricModeCtr[] = {
    PHP_ME(Cryptopp_SymmetricModeCtr, __construct, arginfo_SymmetricModeCtr_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

void init_class_SymmetricModeCtr(TSRMLS_D) {
    init_class_SymmetricModeAbstractChild("ctr", "SymmetricModeCtr", &cryptopp_ce_SymmetricModeCtr, cryptopp_methods_SymmetricModeCtr TSRMLS_CC);
}
/* }}} */

/* {{{ proto SymmetricModeCtr::__construct(Cryptopp\BlockCipherInterface cipher) */
PHP_METHOD(Cryptopp_SymmetricModeCtr, __construct) {
    zval *cipherObject;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &cipherObject, cryptopp_ce_BlockCipherInterface)) {
        return;
    }

    // get needed cipher elements
    CryptoPP::BlockCipher *cipherEncryptor;
    CryptoPP::BlockCipher *cipherDecryptor;
    std::string *modeName;
    bool cipherMustBeDestructed;

    if (!cryptoppSymmetricModeGetCipherElements("ctr", cipherObject, getThis(), &cipherEncryptor, &cipherDecryptor, &modeName, cipherMustBeDestructed TSRMLS_CC)) {
        RETURN_NULL()
    }

    // instanciate mode encryptor/decryptor
    byte dummyIv[cipherEncryptor->BlockSize()];
    Ctr::Encryption *encryptor = new Ctr::Encryption(cipherEncryptor, cipherMustBeDestructed, dummyIv, cipherEncryptor->BlockSize());
    Ctr::Decryption *decryptor = new Ctr::Decryption(cipherEncryptor, false, dummyIv, cipherEncryptor->BlockSize());
    setCryptoppSymmetricModeEncryptorPtr(getThis(), encryptor TSRMLS_CC);
    setCryptoppSymmetricModeDecryptorPtr(getThis(), decryptor TSRMLS_CC);

    if (cipherMustBeDestructed) {
        delete cipherDecryptor;
    }

    zend_update_property_stringl(cryptopp_ce_SymmetricModeAbstract, getThis(), "name", 4, modeName->c_str(), modeName->size() TSRMLS_CC);
    delete modeName;

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
