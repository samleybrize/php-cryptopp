#include "../../../php_cryptopp.h"
#include "../../../exception/php_exception.h"
#include "../block/php_block_cipher_interface.h"
#include "php_authenticated_symmetric_cipher_abstract.h"
#include "php_gcm.h"
#include <zend_exceptions.h>
#include <gcm.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherGcm_construct, 0)
    ZEND_ARG_OBJ_INFO(0, cipher, Cryptopp\\BlockCipherInterface, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_AuthenticatedSymmetricCipherGcm;

static zend_function_entry cryptopp_methods_AuthenticatedSymmetricCipherGcm[] = {
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherGcm, __construct, arginfo_AuthenticatedSymmetricCipherGcm_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

void init_class_AuthenticatedSymmetricCipherGcm(TSRMLS_D) {
    init_class_AuthenticatedSymmetricCipherAbstractChild("gcm", "AuthenticatedSymmetricCipherGcm", &cryptopp_ce_AuthenticatedSymmetricCipherGcm, cryptopp_methods_AuthenticatedSymmetricCipherGcm TSRMLS_CC);
}
/* }}} */

/* {{{ proto AuthenticatedSymmetricCipherGcm::__construct(Cryptopp\BlockCipherInterface cipher) */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherGcm, __construct) {
    zval *cipherObject;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &cipherObject, cryptopp_ce_BlockCipherInterface)) {
        return;
    }

    // get needed cipher elements
    CryptoPP::BlockCipher *cipherEncryptor;
    CryptoPP::BlockCipher *cipherDecryptor;
    std::string *authenticatedCipherName;

    if (!cryptoppAuthenticatedSymmetricCipherGetCipherElements("gcm", cipherObject, getThis(), &cipherEncryptor, &cipherDecryptor, &authenticatedCipherName)) {
        RETURN_NULL()
    } else if (16 != cipherEncryptor->BlockSize()) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\AuthenticatedSymmetricCipherGcm require a block cipher with a block size of 128 bits (16 bytes)");
        RETURN_NULL()
    }

    // instanciate authenticated cipher encryptor/decryptor
    GCM::Encryption *encryptor;
    GCM::Decryption *decryptor;
    encryptor = new GCM::Encryption(cipherEncryptor);
    decryptor = new GCM::Decryption(cipherEncryptor);
    setCryptoppAuthenticatedSymmetricCipherEncryptorPtr(getThis(), encryptor TSRMLS_CC);
    setCryptoppAuthenticatedSymmetricCipherDecryptorPtr(getThis(), decryptor TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, getThis(), "name", 4, authenticatedCipherName->c_str(), authenticatedCipherName->length() TSRMLS_CC);
    delete authenticatedCipherName;

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
