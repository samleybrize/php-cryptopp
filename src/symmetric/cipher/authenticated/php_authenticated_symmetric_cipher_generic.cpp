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
#include "src/mac/mac_proxy.h"
#include "src/mac/php_mac_interface.h"
#include "src/mac/php_mac_abstract.h"
#include "src/utils/zval_utils.h"
#include "src/symmetric/mode/php_symmetric_mode_abstract.h"
#include "src/symmetric/cipher/stream/php_stream_cipher_abstract.h"
#include "src/symmetric/cipher/php_symmetric_transformation_interface.h"
#include "src/symmetric/cipher/symmetric_transformation_proxy.h"
#include "src/symmetric/cipher/symmetric_transformation_user_interface.h"
#include "php_authenticated_symmetric_cipher_interface.h"
#include "php_authenticated_symmetric_cipher_abstract.h"
#include "php_authenticated_symmetric_cipher_generic.h"
#include <zend_exceptions.h>

/* {{{ AuthenticatedSymmetricCipher that take an instance of CryptoPP::SymmetricCipher and an instance of CryptoPP::MessageAuthenticationCode */
AuthenticatedSymmetricCipherGeneric::Base::Base(zval *zCipher, zval *zMac, CryptoPP::SymmetricCipher *cipher, CryptoPP::MessageAuthenticationCode *mac, bool cipherMustBeDestructed, bool macMustBeDestructed)
{
    m_zCipher                   = zCipher;
    m_zMac                      = zMac;
    m_cipher                    = cipher;
    m_mac                       = mac;
    m_funcnameRestart           = makeZval("restart");
    m_cipherMustBeDestructed    = cipherMustBeDestructed;
    m_macMustBeDestructed       = macMustBeDestructed;
}

AuthenticatedSymmetricCipherGeneric::Base::~Base()
{
    zval_ptr_dtor(&m_funcnameRestart);

    if (m_cipherMustBeDestructed) {
        delete m_cipher;
    }

    if (m_macMustBeDestructed) {
        delete m_mac;
    }
}

AuthenticatedSymmetricCipherGeneric::Encryption::Encryption(zval *zCipher, zval *zMac, CryptoPP::SymmetricCipher *cipher, CryptoPP::MessageAuthenticationCode *mac, bool cipherMustBeDestructed, bool macMustBeDestructed)
    : Base(zCipher, zMac, cipher, mac, cipherMustBeDestructed, macMustBeDestructed)
{
    assert(cipher->IsForwardTransformation());
}

AuthenticatedSymmetricCipherGeneric::Decryption::Decryption(zval *zCipher, zval *zMac, CryptoPP::SymmetricCipher *cipher, CryptoPP::MessageAuthenticationCode *mac, bool cipherMustBeDestructed, bool macMustBeDestructed)
    : Base(zCipher, zMac, cipher, mac, cipherMustBeDestructed, macMustBeDestructed)
{
    // can't assert that the cipher is not a forward transformation, because some ciphers use the same transformation for both encryption and decryption
}

void AuthenticatedSymmetricCipherGeneric::Base::ProcessData(byte *outString, const byte *inString, size_t length)
{
    m_cipher->ProcessData(outString, inString, length);

    if (IsForwardTransformation()) {
        // encryption
        m_mac->Update(outString, length);
    } else {
        // decryption
        m_mac->Update(inString, length);
    }
}

void AuthenticatedSymmetricCipherGeneric::Base::SetKeyWithIV(const byte *key, size_t length, const byte *iv, size_t ivLength)
{
    m_cipher->SetKeyWithIV(key, length, iv, ivLength);
    m_mac->Restart();
}

void AuthenticatedSymmetricCipherGeneric::Base::SetKey(const byte *key, size_t length, const CryptoPP::NameValuePairs &params)
{
    m_cipher->SetKey(key, length, params);
    m_mac->Restart();
}

void AuthenticatedSymmetricCipherGeneric::Base::Resynchronize(const byte *iv, int ivLength)
{
    m_cipher->Resynchronize(iv, ivLength);
    m_mac->Restart();
}

void AuthenticatedSymmetricCipherGeneric::Base::SetMacKey(const byte *key, size_t length)
{
    m_mac->SetKey(key, length);
}

bool AuthenticatedSymmetricCipherGeneric::Base::IsValidKeyLength(size_t n) const
{
    return m_cipher->IsValidKeyLength(n);
}

bool AuthenticatedSymmetricCipherGeneric::Base::IsValidIvLength(size_t n)
{
    if (0 != dynamic_cast<SymmetricTransformationUserInterface*>(m_cipher)) {
        return dynamic_cast<SymmetricTransformationUserInterface*>(m_cipher)->IsValidIvLength(n);
    } else {
        return m_cipher->IsResynchronizable() && n >= m_cipher->MinIVLength() && n <= m_cipher->MaxIVLength();
    }
}

bool AuthenticatedSymmetricCipherGeneric::Base::IsValidMacKeyLength(size_t n) const
{
    return m_mac->IsValidKeyLength(n);
}

void AuthenticatedSymmetricCipherGeneric::Base::Restart()
{
    m_mac->Restart();

    if (0 != dynamic_cast<SymmetricTransformationUserInterface*>(m_cipher)) {
        dynamic_cast<SymmetricTransformationUserInterface*>(m_cipher)->Restart();
    }
}
/* }}} */

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherGeneric___construct, 0)
    ZEND_ARG_OBJ_INFO(0, cipher, Cryptopp\\SymmetricTransformationInterface, 0)
    ZEND_ARG_OBJ_INFO(0, mac, Cryptopp\\MacInterface, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherGeneric_getCipher, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherGeneric_getMac, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherGeneric_setMacKey, 0)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_AuthenticatedSymmetricCipherGeneric;

static zend_function_entry cryptopp_methods_AuthenticatedSymmetricCipherGeneric[] = {
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherGeneric, __construct, arginfo_AuthenticatedSymmetricCipherGeneric___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherGeneric, getCipher, arginfo_AuthenticatedSymmetricCipherGeneric_getCipher, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherGeneric, getMac, arginfo_AuthenticatedSymmetricCipherGeneric_getMac, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherGeneric, setMacKey, arginfo_AuthenticatedSymmetricCipherGeneric_setMacKey, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_AuthenticatedSymmetricCipherGeneric(TSRMLS_D) {
    init_class_AuthenticatedSymmetricCipherAbstractChild("generic", "AuthenticatedSymmetricCipherGeneric", &cryptopp_ce_AuthenticatedSymmetricCipherGeneric, cryptopp_methods_AuthenticatedSymmetricCipherGeneric TSRMLS_CC);
    zend_declare_property_null(cryptopp_ce_AuthenticatedSymmetricCipherGeneric, "mac", 3, ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_string(cryptopp_ce_AuthenticatedSymmetricCipherGeneric, "macKey", 6, "", ZEND_ACC_PRIVATE TSRMLS_CC);
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
    std::string **authenticatedCipherFullName,
    bool &cipherMustBeDestructed,
    bool &macMustBeDestructed
    TSRMLS_DC
) {
    cipherMustBeDestructed  = false;
    macMustBeDestructed     = false;

    // cipher
    if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_SymmetricModeAbstract TSRMLS_CC)) {
        // retrieve native objects
        *cipherEncryptor = static_cast<SymmetricModeAbstractContainer *>(zend_object_store_get_object(cipherObject TSRMLS_CC))->encryptor;
        *cipherDecryptor = static_cast<SymmetricModeAbstractContainer *>(zend_object_store_get_object(cipherObject TSRMLS_CC))->decryptor;
    } else if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_StreamCipherAbstract TSRMLS_CC)) {
        // retrieve native objects
        *cipherEncryptor = static_cast<StreamCipherAbstractContainer *>(zend_object_store_get_object(cipherObject TSRMLS_CC))->encryptor;
        *cipherDecryptor = static_cast<StreamCipherAbstractContainer *>(zend_object_store_get_object(cipherObject TSRMLS_CC))->decryptor;
    } else if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_SymmetricTransformationInterface TSRMLS_CC)) {
        // create a proxy to the user php object
        *cipherEncryptor        = new SymmetricTransformationProxy::Encryption(cipherObject TSRMLS_CC);
        *cipherDecryptor        = new SymmetricTransformationProxy::Decryption(cipherObject TSRMLS_CC);
        cipherMustBeDestructed  = true;
    } else {
        // invalid object
        zend_class_entry *ce = zend_get_class_entry(authenticatedCipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Internal error: %s : invalid cipher object", ce->name);
        return false;
    }

    // mac
    if (instanceof_function(Z_OBJCE_P(macObject), cryptopp_ce_MacAbstract TSRMLS_CC)) {
        // retrieve native object
        *mac = static_cast<MacAbstractContainer *>(zend_object_store_get_object(macObject TSRMLS_CC))->mac;
    } else if (instanceof_function(Z_OBJCE_P(macObject), cryptopp_ce_MacInterface TSRMLS_CC)) {
        // create a proxy to the user php object
        *mac                = new MacProxy(macObject TSRMLS_CC);
        macMustBeDestructed = true;
    } else {
        // invalid object
        zend_class_entry *ce = zend_get_class_entry(authenticatedCipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Internal error: %s : invalid mac object", ce->name);
        return false;
    }

    // verify that cipher encryptor/decryptor and mac ptr are not null
    if (NULL == cipherEncryptor || NULL == cipherDecryptor) {
        zend_class_entry *ce = zend_get_class_entry(cipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : parent constructor was not called", ce->name);
        return false;
    } else if (NULL == mac) {
        zend_class_entry *ce = zend_get_class_entry(macObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : parent constructor was not called", ce->name);
        return false;
    }

    // retrieve the name of the cipher
    zval *funcName      = makeZval("getName");
    zval *zCipherName   = call_user_method(cipherObject, funcName TSRMLS_CC);

    // retrieve the name of the mac
    zval *zMacName      = call_user_method(macObject, funcName TSRMLS_CC);

    // build authenticated cipher name with cipher name and mac name
    *authenticatedCipherFullName = new std::string();
    (*authenticatedCipherFullName)->append(Z_STRVAL_P(zCipherName), Z_STRLEN_P(zCipherName));
    (*authenticatedCipherFullName)->append("/");
    (*authenticatedCipherFullName)->append(Z_STRVAL_P(zMacName), Z_STRLEN_P(zMacName));

    zval_ptr_dtor(&zCipherName);
    zval_ptr_dtor(&zMacName);
    zval_ptr_dtor(&funcName);

    return true;
}
/* }}} */

/* {{{ verify that a key size is valid for an AuthenticatedSymmetricCipherGeneric instance */
static bool isCryptoppAuthenticatedSymmetricCipherGenericMacKeyValid(zval *object, CryptoPP::AuthenticatedSymmetricCipher *cipher, int keySize TSRMLS_DC) {
    AuthenticatedSymmetricCipherGeneric::Base *c = dynamic_cast<AuthenticatedSymmetricCipherGeneric::Base*>(cipher);

    if (0 == c) {
        // the cipher is not an instance of AuthenticatedSymmetricCipherGeneric
        return true;
    } else if (!c->IsValidMacKeyLength(keySize)) {
        if (0 == keySize) {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : a MAC key is required", cryptopp_ce_AuthenticatedSymmetricCipherGeneric->name, keySize);
        } else {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : %d is not a valid MAC key length", cryptopp_ce_AuthenticatedSymmetricCipherGeneric->name, keySize);
        }

        return false;
    }

    return true;
}

bool isCryptoppAuthenticatedSymmetricCipherGenericMacKeyValid(zval *object, CryptoPP::AuthenticatedSymmetricCipher *cipher TSRMLS_DC) {
    zval *key   = zend_read_property(cryptopp_ce_AuthenticatedSymmetricCipherGeneric, object, "macKey", 6, 1 TSRMLS_CC);
    int keySize = Z_STRLEN_P(key);

    return isCryptoppAuthenticatedSymmetricCipherGenericMacKeyValid(object, cipher, keySize TSRMLS_CC);
}
/* }}} */

/* {{{ proto AuthenticatedSymmetricCipherGeneric::__construct(Cryptopp\SymmetricTransformationInterface cipher, Cryptopp\MacInterface mac) */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherGeneric, __construct) {
    zval *cipherObject;
    zval *macObject;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "OO", &cipherObject, cryptopp_ce_SymmetricTransformationInterface, &macObject, cryptopp_ce_MacInterface)) {
        return;
    }

    // get needed cipher/mac elements
    CryptoPP::SymmetricCipher *cipherEncryptor;
    CryptoPP::SymmetricCipher *cipherDecryptor;
    CryptoPP::MessageAuthenticationCode *mac;
    std::string *authenticatedCipherName;
    bool cipherMustBeDestructed;
    bool macMustBeDestructed;

    if (!getCipherMacElements(cipherObject, macObject, getThis(), &cipherEncryptor, &cipherDecryptor, &mac, &authenticatedCipherName, cipherMustBeDestructed, macMustBeDestructed TSRMLS_CC)) {
        RETURN_NULL()
    }

    // instanciate authenticated cipher encryptor/decryptor
    AuthenticatedSymmetricCipherGeneric::Encryption *encryptor = new AuthenticatedSymmetricCipherGeneric::Encryption(cipherObject, macObject, cipherEncryptor, mac, cipherMustBeDestructed, macMustBeDestructed);
    AuthenticatedSymmetricCipherGeneric::Decryption *decryptor = new AuthenticatedSymmetricCipherGeneric::Decryption(cipherObject, macObject, cipherDecryptor, mac, cipherMustBeDestructed, false);
    setCryptoppAuthenticatedSymmetricCipherEncryptorPtr(getThis(), encryptor TSRMLS_CC);
    setCryptoppAuthenticatedSymmetricCipherDecryptorPtr(getThis(), decryptor TSRMLS_CC);

    zend_update_property_stringl(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, getThis(), "name", 4, authenticatedCipherName->c_str(), authenticatedCipherName->length() TSRMLS_CC);
    delete authenticatedCipherName;

    // hold the cipher/mac objects
    zend_update_property(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, getThis(), "cipher", 6, cipherObject TSRMLS_CC);
    zend_update_property(cryptopp_ce_AuthenticatedSymmetricCipherGeneric, getThis(), "mac", 3, macObject TSRMLS_CC);
}
/* }}} */

/* {{{ proto AuthenticatedSymmetricCipherGeneric::getCipher() */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherGeneric, getCipher) {
    zval *cipherObject = zend_read_property(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, getThis(), "cipher", 6, 0 TSRMLS_CC);
    RETURN_ZVAL(cipherObject, 1, 0)
}
/* }}} */

/* {{{ proto AuthenticatedSymmetricCipherGeneric::getMac() */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherGeneric, getMac) {
    zval *macObject = zend_read_property(cryptopp_ce_AuthenticatedSymmetricCipherGeneric, getThis(), "mac", 3, 0 TSRMLS_CC);
    RETURN_ZVAL(macObject, 1, 0)
}
/* }}} */

/* {{{ proto void AuthenticatedSymmetricCipherGeneric::setMacKey(string key)
   Sets the MAC key */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherGeneric, setMacKey) {
    char *key   = NULL;
    int keySize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &keySize)) {
        return;
    }

    CryptoPP::AuthenticatedSymmetricCipher *encryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    CryptoPP::AuthenticatedSymmetricCipher *decryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);

    if (!isCryptoppAuthenticatedSymmetricCipherGenericMacKeyValid(getThis(), encryptor, keySize TSRMLS_CC)) {
        RETURN_FALSE;
    }

    // set the key on both the php object and the native cryptopp object
    zend_update_property_stringl(cryptopp_ce_AuthenticatedSymmetricCipherGeneric, getThis(), "macKey", 6, key, keySize TSRMLS_CC);
    dynamic_cast<AuthenticatedSymmetricCipherGeneric::Base*>(encryptor)->SetMacKey(reinterpret_cast<byte*>(key), keySize);
    dynamic_cast<AuthenticatedSymmetricCipherGeneric::Base*>(decryptor)->SetMacKey(reinterpret_cast<byte*>(key), keySize);
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
