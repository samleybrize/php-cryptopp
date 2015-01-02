#include "../../../php_cryptopp.h"
#include "../../../exception/php_exception.h"
#include "../../../mac/php_mac_interface.h"
#include "../../../mac/php_mac_abstract.h"
#include "../../mode/php_symmetric_mode_abstract.h"
#include "../stream/php_stream_cipher_abstract.h"
#include "../php_symmetric_transformation_interface.h"
#include "../symmetric_transformation_proxy.h"
#include "php_authenticated_symmetric_cipher_interface.h"
#include "php_authenticated_symmetric_cipher_abstract.h"
#include "php_authenticated_symmetric_cipher_generic.h"
#include <zend_exceptions.h>

// TODO AuthenticatedSymmetricCipherGeneric
AuthenticatedSymmetricCipherGeneric::Base::Base(CryptoPP::SymmetricCipher *cipher, CryptoPP::MessageAuthenticationCode *mac)
{
    m_cipher    = cipher;
    m_mac       = mac;
}

AuthenticatedSymmetricCipherGeneric::Encryption::Encryption(CryptoPP::SymmetricCipher *cipher, CryptoPP::MessageAuthenticationCode *mac) : Base(cipher, mac)
{
    assert(cipher->IsForwardTransformation());
}

AuthenticatedSymmetricCipherGeneric::Decryption::Decryption(CryptoPP::SymmetricCipher *cipher, CryptoPP::MessageAuthenticationCode *mac) : Base(cipher, mac)
{
    assert(!cipher->IsForwardTransformation());
}

unsigned int AuthenticatedSymmetricCipherGeneric::Base::MandatoryBlockSize() const
{
    return m_cipher->MandatoryBlockSize();
}

unsigned int AuthenticatedSymmetricCipherGeneric::Base::OptimalBlockSize() const
{
    return m_cipher->OptimalBlockSize();
}

unsigned int AuthenticatedSymmetricCipherGeneric::Base::DigestSize() const
{
    return m_mac->DigestSize();
}

unsigned int AuthenticatedSymmetricCipherGeneric::Base::MinLastBlockSize() const
{
    return m_cipher->MinLastBlockSize();
}

void AuthenticatedSymmetricCipherGeneric::Base::ProcessData(byte *outString, const byte *inString, size_t length)
{
    m_cipher->ProcessData(outString, inString, length);

    if (IsForwardTransformation()) {
        m_mac->Update(outString, length);
    } else {
        m_mac->Update(inString, length);
    }
}

void AuthenticatedSymmetricCipherGeneric::Base::Update(const byte *input, size_t length)
{
    m_mac->Update(input, length);
}

void AuthenticatedSymmetricCipherGeneric::Base::TruncatedFinal(byte *digest, size_t digestSize)
{
    m_mac->TruncatedFinal(digest, digestSize);
}

void AuthenticatedSymmetricCipherGeneric::Base::Restart(const byte *iv, int ivLength)
{
    m_cipher->Resynchronize(iv, ivLength);
    m_mac->Restart();
}

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherGeneric_construct, 0)
    ZEND_ARG_OBJ_INFO(0, cipher, Cryptopp\\SymmetricTransformationInterface, 0)
    ZEND_ARG_OBJ_INFO(0, mac, Cryptopp\\MacInterface, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_AuthenticatedSymmetricCipherGeneric;

static zend_function_entry cryptopp_methods_AuthenticatedSymmetricCipherGeneric[] = {
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherGeneric, __construct, arginfo_AuthenticatedSymmetricCipherGeneric_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

void init_class_AuthenticatedSymmetricCipherGeneric(TSRMLS_D) {
    init_class_AuthenticatedSymmetricCipherAbstractChild("generic", "AuthenticatedSymmetricCipherGeneric", &cryptopp_ce_AuthenticatedSymmetricCipherGeneric, cryptopp_methods_AuthenticatedSymmetricCipherGeneric TSRMLS_CC);
    zend_declare_property_null(cryptopp_ce_AuthenticatedSymmetricCipherGeneric, "mac", 3, ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ Get needed cipher/mac elements to build an authenticated cipher object */
static bool getCipherMacElements(
    zval *cipherObject,
    zval *macObject,
    zval *authenticatedCipherObject,
    CryptoPP::SymmetricCipher **cipherEncryptor,
    CryptoPP::SymmetricCipher **cipherDecryptor,
    CryptoPP::MessageAuthenticationCode **mac,
    std::string **authenticatedCipherFullName
) {
    // cipher
    if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_SymmetricModeAbstract)) {
        // retrieve native objects
        *cipherEncryptor = static_cast<SymmetricModeAbstractContainer *>(zend_object_store_get_object(cipherObject TSRMLS_CC))->encryptor;
        *cipherDecryptor = static_cast<SymmetricModeAbstractContainer *>(zend_object_store_get_object(cipherObject TSRMLS_CC))->decryptor;
    } else if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_StreamCipherAbstract)) {
        // retrieve native objects
        *cipherEncryptor = static_cast<StreamCipherAbstractContainer *>(zend_object_store_get_object(cipherObject TSRMLS_CC))->encryptor;
        *cipherDecryptor = static_cast<StreamCipherAbstractContainer *>(zend_object_store_get_object(cipherObject TSRMLS_CC))->decryptor;
    } else if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_SymmetricTransformationInterface)) {
        // create a proxy to the user php object
        *cipherEncryptor = new SymmetricTransformationProxy::Encryption(cipherObject);
        *cipherDecryptor = new SymmetricTransformationProxy::Decryption(cipherObject);
    } else {
        // invalid object
        zend_class_entry *ce;
        ce  = zend_get_class_entry(authenticatedCipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Internal error: %s : invalid cipher object", ce->name);
        return false;
    }

    // mac
    if (instanceof_function(Z_OBJCE_P(macObject), cryptopp_ce_MacAbstract)) {
        // retrieve native object
        *mac = static_cast<MacAbstractContainer *>(zend_object_store_get_object(cipherObject TSRMLS_CC))->mac;
    } else if (instanceof_function(Z_OBJCE_P(macObject), cryptopp_ce_MacInterface)) {
        // TODO create a proxy to the user php object
        //*mac = new MacProxy(macObject);
    } else {
        // invalid object
        zend_class_entry *ce;
        ce  = zend_get_class_entry(authenticatedCipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Internal error: %s : invalid mac object", ce->name);
        return false;
    }

    // verify that cipher encryptor/decryptor and mac ptr are not null
    if (NULL == cipherEncryptor || NULL == cipherDecryptor) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(cipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : parent constructor was not called", ce->name);
        return false;
    } else if (NULL == mac) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(macObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : parent constructor was not called", ce->name);
        return false;
    }

    // retrieve the name of the cipher
    zval *zCipherName;
    zval *funcName;
    MAKE_STD_ZVAL(zCipherName);
    MAKE_STD_ZVAL(funcName);
    ZVAL_STRING(funcName, "getName", 1);
    call_user_function(NULL, &cipherObject, funcName, zCipherName, 0, NULL TSRMLS_CC);

    // retrieve the name of the mac
    zval *zMacName;
    MAKE_STD_ZVAL(zMacName);
    call_user_function(NULL, &macObject, funcName, zMacName, 0, NULL TSRMLS_CC);

    // build authenticated cipher name with cipher name and mac name
    *authenticatedCipherFullName = new std::string();
    (*authenticatedCipherFullName)->append(Z_STRVAL_P(zCipherName), Z_STRLEN_P(zCipherName));
    (*authenticatedCipherFullName)->append("/");
    (*authenticatedCipherFullName)->append(Z_STRVAL_P(zMacName), Z_STRLEN_P(zMacName));

    zval_dtor(zCipherName);
    zval_dtor(zMacName);
    zval_dtor(funcName);

    return true;
}
/* }}} */

/* {{{ proto AuthenticatedSymmetricCipherGeneric::__construct(Cryptopp\SymmetricTransformationInterface cipher, Cryptopp\MacInterface mac) */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherGeneric, __construct) {
    zval *cipherObject;
    zval *macObject;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "OO", &cipherObject, cryptopp_ce_SymmetricTransformationInterface, &macObject, cryptopp_ce_MacInterface)) {
        return;
    }

    // TODO how to handle setKey()? (mac may not accept the same key than the cipher)

    // get needed cipher/mac elements
    CryptoPP::SymmetricCipher *cipherEncryptor;
    CryptoPP::SymmetricCipher *cipherDecryptor;
    CryptoPP::MessageAuthenticationCode *mac;
    std::string *authenticatedCipherName;

    if (!getCipherMacElements(cipherObject, macObject, getThis(), &cipherEncryptor, &cipherDecryptor, &mac, &authenticatedCipherName)) {
        RETURN_NULL()
    }

    // instanciate authenticated cipher encryptor/decryptor
    AuthenticatedSymmetricCipherGeneric::Encryption *encryptor;
    AuthenticatedSymmetricCipherGeneric::Decryption *decryptor;
    encryptor = new AuthenticatedSymmetricCipherGeneric::Encryption(cipherEncryptor, mac);
    decryptor = new AuthenticatedSymmetricCipherGeneric::Decryption(cipherDecryptor, mac);
    setCryptoppAuthenticatedSymmetricCipherEncryptorPtr(getThis(), encryptor TSRMLS_CC);
    setCryptoppAuthenticatedSymmetricCipherDecryptorPtr(getThis(), decryptor TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, getThis(), "name", 4, authenticatedCipherName->c_str(), authenticatedCipherName->length() TSRMLS_CC);
    delete authenticatedCipherName;

    // hold the cipher/mac objects
    zend_update_property(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, getThis(), "cipher", 6, cipherObject TSRMLS_CC);
    zend_update_property(cryptopp_ce_AuthenticatedSymmetricCipherGeneric, getThis(), "mac", 3, macObject TSRMLS_CC);
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
