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
#include "src/utils/zval_utils.h"
#include "php_authenticated_symmetric_cipher_abstract.h"
#include "php_ccm.h"
#include <zend_exceptions.h>
#include <ccm.h>

/* {{{ CCM::Base::Base */
CCM::Base::Base(CryptoPP::BlockCipher *cipher, bool cipherMustBeDestructed TSRMLS_DC)
    : m_cipher(cipher)
    , m_cipherMustBeDestructed(cipherMustBeDestructed)
{
    SET_M_TSRMLS_C()
}
/* }}} */

/* {{{ CCM::Base::~Base */
CCM::Base::~Base()
{
    if (m_cipherMustBeDestructed) {
        delete m_cipher;
    }
}
/* }}} */

/* {{{ CCM::Base::SetDigestSize */
void CCM::Base::SetDigestSize(int digestSize)
{
    if (digestSize % 2 > 0 || digestSize < 4 || digestSize > 16) {
        zend_throw_exception_ex(getCryptoppException(), 0 M_TSRMLS_CC, (char*)"Cryptopp\\AuthenticatedSymmetricCipherCcm : digest size must be 4, 6, 8, 10, 12, 14, or 16");
    }

    m_digestSize = digestSize;
}
/* }}} */

/* {{{ CCM::Base::ProcessData */
void CCM::Base::ProcessData(byte *outString, const byte *inString, size_t length)
{
    if (m_totalMessageLength + length > m_messageLength) {
        zend_throw_exception_ex(getCryptoppException(), 0 M_TSRMLS_CC, (char*)"Cryptopp\\AuthenticatedSymmetricCipherCcm : message length doesn't match that given in specifyDataSize (%d expected, %d given)", m_messageLength, m_totalMessageLength + length);
        throw false;
    }

    CryptoPP::AuthenticatedSymmetricCipherBase::ProcessData(outString, inString, length);
}
/* }}} */

/* {{{ CCM::Base::Update */
void CCM::Base::Update(const byte *input, size_t length)
{
    if (m_totalHeaderLength + length > m_aadLength) {
        zend_throw_exception_ex(getCryptoppException(), 0 M_TSRMLS_CC, (char*)"Cryptopp\\AuthenticatedSymmetricCipherCcm : AAD length doesn't match that given in specifyDataSize (%d expected, %d given)", m_aadLength, m_totalHeaderLength + length);
        throw false;
    }

    CryptoPP::AuthenticatedSymmetricCipherBase::Update(input, length);
}
/* }}} */

/* {{{ CCM::Base::Final */
void CCM::Base::Final(byte *digest)
{
    if (m_totalMessageLength != m_messageLength) {
        zend_throw_exception_ex(getCryptoppException(), 0 M_TSRMLS_CC, (char*)"Cryptopp\\AuthenticatedSymmetricCipherCcm : message length doesn't match that given in specifyDataSize (%d expected, %d given)", m_messageLength, m_totalMessageLength);
        throw false;
    } else if (m_totalHeaderLength != m_aadLength) {
        zend_throw_exception_ex(getCryptoppException(), 0 M_TSRMLS_CC, (char*)"Cryptopp\\AuthenticatedSymmetricCipherCcm : AAD length doesn't match that given in specifyDataSize (%d expected, %d given)", m_aadLength, m_totalHeaderLength);
        throw false;
    }

    CryptoPP::AuthenticatedSymmetricCipherBase::Final(digest);
}
/* }}} */

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherCcm_construct, 0)
    ZEND_ARG_OBJ_INFO(0, cipher, Cryptopp\\BlockCipherInterface, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherCcm_setDigestSize, 0)
    ZEND_ARG_INFO(0, digestSize)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherCcm_specifyDataSize, 0)
    ZEND_ARG_INFO(0, dataSize)
    ZEND_ARG_INFO(0, aadSize)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_AuthenticatedSymmetricCipherCcm;

static zend_function_entry cryptopp_methods_AuthenticatedSymmetricCipherCcm[] = {
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherCcm, __construct, arginfo_AuthenticatedSymmetricCipherCcm_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherCcm, setDigestSize, arginfo_AuthenticatedSymmetricCipherCcm_setDigestSize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherCcm, specifyDataSize, arginfo_AuthenticatedSymmetricCipherCcm_specifyDataSize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_AuthenticatedSymmetricCipherCcm(TSRMLS_D) {
    init_class_AuthenticatedSymmetricCipherAbstractChild("ccm", "AuthenticatedSymmetricCipherCcm", &cryptopp_ce_AuthenticatedSymmetricCipherCcm, cryptopp_methods_AuthenticatedSymmetricCipherCcm TSRMLS_CC);
    zend_declare_property_long(cryptopp_ce_AuthenticatedSymmetricCipherCcm, "digestSize", 10, 16,  ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_long(cryptopp_ce_AuthenticatedSymmetricCipherCcm, "dataSize", 8, 0,  ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_long(cryptopp_ce_AuthenticatedSymmetricCipherCcm, "aadSize", 7, 0,  ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ setDataSizeAndDigestSize
   sets the digest size, data size and AAD size of the native cipher objects of a CCM php object */
static void setDataSizeAndDigestSize(zval *object, CCM::Base *encryptor, CCM::Base *decryptor TSRMLS_DC) {
    zval *zDigestSize   = zend_read_property(cryptopp_ce_AuthenticatedSymmetricCipherCcm, object, "digestSize", 10, 1 TSRMLS_CC);
    zval *zDataSize     = zend_read_property(cryptopp_ce_AuthenticatedSymmetricCipherCcm, object, "dataSize", 8, 1 TSRMLS_CC);
    zval *zAadSize      = zend_read_property(cryptopp_ce_AuthenticatedSymmetricCipherCcm, object, "aadSize", 7, 1 TSRMLS_CC);

    // restart encryption/decryption
    zval *funcname  = makeZval("restart");
    zval *output    = call_user_method(object, funcname TSRMLS_CC);
    zval_ptr_dtor(&funcname);
    zval_ptr_dtor(&output);

    // set digest size, data size and aad size
    encryptor->SetDigestSize(Z_LVAL_P(zDigestSize));
    decryptor->SetDigestSize(Z_LVAL_P(zDigestSize));
    encryptor->SpecifyDataLengths(Z_LVAL_P(zAadSize), Z_LVAL_P(zDataSize));
    decryptor->SpecifyDataLengths(Z_LVAL_P(zAadSize), Z_LVAL_P(zDataSize));
}
/* }}} */

/* {{{ proto AuthenticatedSymmetricCipherCcm::__construct(Cryptopp\BlockCipherInterface cipher) */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherCcm, __construct) {
    zval *cipherObject;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &cipherObject, cryptopp_ce_BlockCipherInterface)) {
        return;
    }

    // get needed cipher elements
    CryptoPP::BlockCipher *cipherEncryptor;
    CryptoPP::BlockCipher *cipherDecryptor;
    std::string *authenticatedCipherName;
    bool cipherMustBeDestructed;

    if (!cryptoppAuthenticatedSymmetricCipherGetCipherElements("ccm", cipherObject, getThis(), &cipherEncryptor, &cipherDecryptor, &authenticatedCipherName, cipherMustBeDestructed TSRMLS_CC)) {
        RETURN_NULL()
    } else if (16 != cipherEncryptor->BlockSize()) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\AuthenticatedSymmetricCipherCcm require a block cipher with a block size of 128 bits (16 bytes)");
        delete cipherEncryptor;
        delete cipherDecryptor;
        delete authenticatedCipherName;
        RETURN_NULL()
    }

    if (cipherMustBeDestructed) {
        delete cipherDecryptor;
    }

    // instanciate authenticated cipher encryptor/decryptor
    CCM::Encryption *encryptor = new CCM::Encryption(cipherEncryptor, cipherMustBeDestructed TSRMLS_CC);
    CCM::Decryption *decryptor = new CCM::Decryption(cipherEncryptor, false TSRMLS_CC);
    setCryptoppAuthenticatedSymmetricCipherEncryptorPtr(getThis(), encryptor TSRMLS_CC);
    setCryptoppAuthenticatedSymmetricCipherDecryptorPtr(getThis(), decryptor TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, getThis(), "name", 4, authenticatedCipherName->c_str(), authenticatedCipherName->length() TSRMLS_CC);
    delete authenticatedCipherName;

    // defaults
    encryptor->SetDigestSize(16);
    decryptor->SetDigestSize(16);

    // hold the cipher object. if not, it can be deleted and associated encryptor/decryptor objects will be deleted too
    zend_update_property(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, getThis(), "cipher", 6, cipherObject TSRMLS_CC);
}
/* }}} */

/* {{{ proto void AuthenticatedSymmetricCipherCcm::setDigestSize(int digestSize)
   Sets the digest size */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherCcm, setDigestSize) {
    long digestSize;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &digestSize)) {
        return;
    }

    CryptoPP::AuthenticatedSymmetricCipher *encryptor;
    CryptoPP::AuthenticatedSymmetricCipher *decryptor;
    CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)
    CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor)

    // check digest size
    if (digestSize % 2 > 0 || digestSize < 4 || digestSize > 16) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\AuthenticatedSymmetricCipherCcm : digest size must be 4, 6, 8, 10, 12, 14, or 16");
        RETURN_FALSE
    }

    // check key/iv
    if (!isCryptoppAuthenticatedSymmetricCipherKeyValid(getThis(), encryptor TSRMLS_CC) || !isCryptoppAuthenticatedSymmetricCipherIvValid(getThis(), encryptor TSRMLS_CC)) {
        RETURN_FALSE
    }

    zend_update_property_long(cryptopp_ce_AuthenticatedSymmetricCipherCcm, getThis(), "digestSize", 10, digestSize TSRMLS_CC);
    setDataSizeAndDigestSize(getThis(), static_cast<CCM::Base *>(encryptor), static_cast<CCM::Base *>(decryptor) TSRMLS_CC);
}
/* }}} */

/* {{{ proto void AuthenticatedSymmetricCipherCcm::specifyDataSize(int dataSize, int aadSize)
   Specify data and AAD size */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherCcm, specifyDataSize) {
    long dataSize;
    long aadSize;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &dataSize, &aadSize)) {
        return;
    }

    CryptoPP::AuthenticatedSymmetricCipher *encryptor;
    CryptoPP::AuthenticatedSymmetricCipher *decryptor;
    CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)
    CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor)

    // check data size and aad size
    if (dataSize < 0) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\AuthenticatedSymmetricCipherCcm : data size cannot be lower than 0");
    } else if (aadSize < 0) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\AuthenticatedSymmetricCipherCcm : AAD size cannot be lower than 0");
    }

    // check key/iv
    if (!isCryptoppAuthenticatedSymmetricCipherKeyValid(getThis(), encryptor TSRMLS_CC) || !isCryptoppAuthenticatedSymmetricCipherIvValid(getThis(), encryptor TSRMLS_CC)) {
        RETURN_FALSE
    }

    zend_update_property_long(cryptopp_ce_AuthenticatedSymmetricCipherCcm, getThis(), "dataSize", 8, dataSize TSRMLS_CC);
    zend_update_property_long(cryptopp_ce_AuthenticatedSymmetricCipherCcm, getThis(), "aadSize", 7, aadSize TSRMLS_CC);
    setDataSizeAndDigestSize(getThis(), static_cast<CCM::Base *>(encryptor), static_cast<CCM::Base *>(decryptor) TSRMLS_CC);
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
