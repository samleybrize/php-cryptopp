#include "../php_cryptopp.h"
#include "../exception/php_exception.h"
#include "../exception/php_mac_verification_failed_exception.h"
#include "../padding/php_padding_interface.h"
#include "../padding/php_pkcs7.h"
#include "../symmetric/cipher/authenticated/php_authenticated_symmetric_cipher_interface.h"
#include "../symmetric/cipher/authenticated/php_authenticated_symmetric_cipher_abstract.h"
#include "../symmetric/cipher/authenticated/authenticated_symmetric_cipher_proxy.h"
#include "../utils/zend_object_utils.h"
#include "../utils/zval_utils.h"
#include "php_authenticated_symmetric_transformation_filter.h"
#include <exception>
#include <filters.h>
#include <zend_exceptions.h>

/* {{{ fork of CryptoPP::AuthenticatedEncryptionFilter to support padding schemes as objects */
AuthenticatedEncryptionFilter::AuthenticatedEncryptionFilter(CryptoPP::AuthenticatedSymmetricCipher &c, zval *paddingObject, bool cipherMustBeDestructed TSRMLS_DC)
    : SymmetricTransformationFilter(c, paddingObject, cipherMustBeDestructed TSRMLS_CC, true)
    , m_hf(c, new CryptoPP::OutputProxy(*this, false), false, -1, CryptoPP::DEFAULT_CHANNEL, CryptoPP::DEFAULT_CHANNEL)
{
    // initialization copied from CryptoPP::AuthenticatedEncryptionFilter
    assert(c.IsForwardTransformation());
}
/* }}} */

/* {{{ copied from CryptoPP::AuthenticatedEncryptionFilter */
void AuthenticatedEncryptionFilter::IsolatedInitialize(const CryptoPP::NameValuePairs &parameters)
{
    m_hf.IsolatedInitialize(parameters);
    SymmetricTransformationFilter::IsolatedInitialize(parameters);
}

byte * AuthenticatedEncryptionFilter::ChannelCreatePutSpace(const std::string &channel, size_t &size)
{
    if (channel.empty()) {
        return SymmetricTransformationFilter::CreatePutSpace(size);
    }

    if (channel == CryptoPP::AAD_CHANNEL) {
        return m_hf.CreatePutSpace(size);
    }

    throw InvalidChannelName("AuthenticatedEncryptionFilter", channel);
}

size_t AuthenticatedEncryptionFilter::ChannelPut2(const std::string &channel, const byte *begin, size_t length, int messageEnd, bool blocking)
{
    if (channel.empty()) {
        return SymmetricTransformationFilter::Put2(begin, length, messageEnd, blocking);
    }

    if (channel == CryptoPP::AAD_CHANNEL) {
        return m_hf.Put2(begin, length, 0, blocking);
    }

    throw InvalidChannelName("AuthenticatedEncryptionFilter", channel);
}

void AuthenticatedEncryptionFilter::LastPut(const byte *inString, size_t length)
{
    SymmetricTransformationFilter::LastPut(inString, length);
    m_hf.MessageEnd();
}
/* }}} */

/* {{{ fork of CryptoPP::HashVerificationFilter to return some protected properties */
size_t HashVerificationFilter::FirstSize()
{
    return m_firstSize;
}

size_t HashVerificationFilter::LastSize()
{
    return m_lastSize;
}
/* }}} */

/* {{{ fork of CryptoPP::AuthenticatedDecryptionFilter to support padding schemes as objects */
AuthenticatedDecryptionFilter::AuthenticatedDecryptionFilter(CryptoPP::AuthenticatedSymmetricCipher &c, zval *paddingObject, bool cipherMustBeDestructed TSRMLS_DC, CryptoPP::word32 flags)
    : FilterWithBufferedInput(NULL)
    , m_hashVerifier(c, new CryptoPP::OutputProxy(*this, false))
    , m_streamFilter(c, paddingObject, cipherMustBeDestructed TSRMLS_CC, true)
{
    m_streamFilter.Attach(new CryptoPP::OutputProxy(*this, false));

    // initialization copied from CryptoPP::AuthenticatedDecryptionFilter
    assert(!c.IsForwardTransformation() || c.IsSelfInverting());
    IsolatedInitialize(CryptoPP::MakeParameters(CryptoPP::Name::AuthenticatedDecryptionFilterFlags(), flags)(CryptoPP::Name::TruncatedDigestSize(), -1));
}

void AuthenticatedDecryptionFilter::InitializeDerivedAndReturnNewSizes(const CryptoPP::NameValuePairs &parameters, size_t &firstSize, size_t &blockSize, size_t &lastSize)
{
    CryptoPP::word32 flags = parameters.GetValueWithDefault(CryptoPP::Name::AuthenticatedDecryptionFilterFlags(), (CryptoPP::word32)DEFAULT_FLAGS);

    m_hashVerifier.Initialize(CryptoPP::CombinedNameValuePairs(parameters, CryptoPP::MakeParameters(CryptoPP::Name::HashVerificationFilterFlags(), flags)));
    m_streamFilter.Initialize(parameters);

    firstSize   = m_hashVerifier.FirstSize();
    blockSize   = 1;
    lastSize    = m_hashVerifier.LastSize();
}

byte * AuthenticatedDecryptionFilter::ChannelCreatePutSpace(const std::string &channel, size_t &size)
{
    if (channel.empty()) {
        return m_streamFilter.CreatePutSpace(size);
    }

    if (channel == CryptoPP::AAD_CHANNEL) {
        return m_hashVerifier.CreatePutSpace(size);
    }

    throw InvalidChannelName("AuthenticatedDecryptionFilter", channel);
}

size_t AuthenticatedDecryptionFilter::ChannelPut2(const std::string &channel, const byte *begin, size_t length, int messageEnd, bool blocking)
{
    if (channel.empty()) {
        if (m_lastSize > 0) {
            m_hashVerifier.ForceNextPut();
        }

        return FilterWithBufferedInput::Put2(begin, length, messageEnd, blocking);
    }

    if (channel == CryptoPP::AAD_CHANNEL) {
        return m_hashVerifier.Put2(begin, length, 0, blocking);
    }

    throw InvalidChannelName("AuthenticatedDecryptionFilter", channel);
}

void AuthenticatedDecryptionFilter::FirstPut(const byte *inString)
{
    m_hashVerifier.Put(inString, m_firstSize);
}

void AuthenticatedDecryptionFilter::NextPutMultiple(const byte *inString, size_t length)
{
    m_streamFilter.Put(inString, length);
}

void AuthenticatedDecryptionFilter::LastPut(const byte *inString, size_t length)
{
    m_streamFilter.MessageEnd();
    m_hashVerifier.PutMessageEnd(inString, length);
}
/* }}} */

/* {{{ arg info */
ZEND_BEGIN_ARG_INFO_EX(arginfo_AuthenticatedSymmetricTransformationFilter___construct, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, cipher, Cryptopp\\AuthenticatedSymmetricCipherInterface, 0)
    ZEND_ARG_OBJ_INFO(0, padding, Cryptopp\\PaddingInterface, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricTransformationFilter___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricTransformationFilter___sleep, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricTransformationFilter_getCipher, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricTransformationFilter_getPadding, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_AuthenticatedSymmetricTransformationFilter_encryptString, 0, 0, 0)
    ZEND_ARG_INFO(0, data)
    ZEND_ARG_INFO(0, aad)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_AuthenticatedSymmetricTransformationFilter_decryptString, 0, 0, 0)
    ZEND_ARG_INFO(0, ciphertext)
    ZEND_ARG_INFO(0, aad)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ custom object free handler */
void AuthenticatedSymmetricTransformationFilter_free_storage(void *object TSRMLS_DC) {
    AuthenticatedSymmetricTransformationFilterContainer *obj = static_cast<AuthenticatedSymmetricTransformationFilterContainer *>(object);
    delete obj->stfEncryptor;
    delete obj->stfDecryptor;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}
/* }}} */

/* {{{ PHP class declaration */
zend_object_handlers AuthenticatedSymmetricTransformationFilter_object_handlers;
zend_class_entry *cryptopp_ce_AuthenticatedSymmetricTransformationFilter;

static zend_function_entry cryptopp_methods_AuthenticatedSymmetricTransformationFilter[] = {
    PHP_ME(Cryptopp_AuthenticatedSymmetricTransformationFilter, __construct, arginfo_AuthenticatedSymmetricTransformationFilter___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Cryptopp_AuthenticatedSymmetricTransformationFilter, __sleep, arginfo_AuthenticatedSymmetricTransformationFilter___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricTransformationFilter, __wakeup, arginfo_AuthenticatedSymmetricTransformationFilter___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricTransformationFilter, getCipher, arginfo_AuthenticatedSymmetricTransformationFilter_getCipher, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricTransformationFilter, getPadding, arginfo_AuthenticatedSymmetricTransformationFilter_getPadding, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricTransformationFilter, encryptString, arginfo_AuthenticatedSymmetricTransformationFilter_encryptString, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricTransformationFilter, decryptString, arginfo_AuthenticatedSymmetricTransformationFilter_decryptString, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_AuthenticatedSymmetricTransformationFilter(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "AuthenticatedSymmetricTransformationFilter", cryptopp_methods_AuthenticatedSymmetricTransformationFilter);
    cryptopp_ce_AuthenticatedSymmetricTransformationFilter = zend_register_internal_class(&ce TSRMLS_CC);

    cryptopp_ce_AuthenticatedSymmetricTransformationFilter->create_object = zend_custom_create_handler<AuthenticatedSymmetricTransformationFilterContainer, AuthenticatedSymmetricTransformationFilter_free_storage, &AuthenticatedSymmetricTransformationFilter_object_handlers>;
    memcpy(&AuthenticatedSymmetricTransformationFilter_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    AuthenticatedSymmetricTransformationFilter_object_handlers.clone_obj = NULL;

    zend_declare_property_null(cryptopp_ce_AuthenticatedSymmetricTransformationFilter, "cipher", 6, ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_null(cryptopp_ce_AuthenticatedSymmetricTransformationFilter, "padding", 7, ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ get the pointer to the native stf encryptor object of the php class */
static AuthenticatedEncryptionFilter *getCryptoppAuthenticatedSymmetricTransformationFilterEncryptorPtr(zval *this_ptr TSRMLS_DC) {
    AuthenticatedEncryptionFilter *stf;
    stf = static_cast<AuthenticatedSymmetricTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfEncryptor;

    if (NULL == stf) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\AuthenticatedSymmetricTransformationFilter : constructor was not called");
    }

    return stf;
}
/* }}} */

/* {{{ get the pointer to the native stf decryptor object of the php class */
static AuthenticatedDecryptionFilter *getCryptoppAuthenticatedSymmetricTransformationFilterDecryptorPtr(zval *this_ptr TSRMLS_DC) {
    AuthenticatedDecryptionFilter *stf;
    stf = static_cast<AuthenticatedSymmetricTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfDecryptor;

    if (NULL == stf) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\AuthenticatedSymmetricTransformationFilter : constructor was not called");
    }

    return stf;
}
/* }}} */

/* {{{ set the pointer to the native stf encryptor object of the php class */
static void setCryptoppAuthenticatedSymmetricTransformationFilterEncryptorPtr(zval *this_ptr, AuthenticatedEncryptionFilter *nativePtr TSRMLS_DC) {
    static_cast<AuthenticatedSymmetricTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfEncryptor = nativePtr;
}
/* }}} */

/* {{{ set the pointer to the native stf decryptor object of the php class */
static void setCryptoppAuthenticatedSymmetricTransformationFilterDecryptorPtr(zval *this_ptr, AuthenticatedDecryptionFilter *nativePtr TSRMLS_DC) {
    static_cast<AuthenticatedSymmetricTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->stfDecryptor = nativePtr;
}
/* }}} */

/* {{{ indicates if the native cipher object holded by a stf object is valid */
static bool isNativeCipherObjectValid(zval *stfObject TSRMLS_DC) {
    zval *cipherObject;
    cipherObject = zend_read_property(cryptopp_ce_AuthenticatedSymmetricTransformationFilter, stfObject, "cipher", 6, 0 TSRMLS_CC);

    if (IS_OBJECT != Z_TYPE_P(cipherObject)) {
        // not an object
        return false;
    } else if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_AuthenticatedSymmetricCipherAbstract TSRMLS_CC)) {
        // AuthenticatedSymmetricCipherAbstract
        CryptoPP::AuthenticatedSymmetricCipher *cipherEncryptor;
        cipherEncryptor = getCryptoppAuthenticatedSymmetricCipherEncryptorPtr(cipherObject TSRMLS_CC);

        if (!isCryptoppAuthenticatedSymmetricCipherKeyValid(cipherObject, cipherEncryptor TSRMLS_CC) || !isCryptoppAuthenticatedSymmetricCipherIvValid(cipherObject, cipherEncryptor TSRMLS_CC)) {
            return false;
        }
    }

    return true;
}
/* }}} */

/* {{{ restart the cipher holded by a AuthenticatedSymmetricTransformationFilter php object */
static void restartCipherObject(zval *stfObject TSRMLS_DC) {
    zval *cipherObject;
    cipherObject    = zend_read_property(cryptopp_ce_AuthenticatedSymmetricTransformationFilter, stfObject, "cipher", 6, 0 TSRMLS_CC);

    zval *funcName  = makeZval("restart");
    zval *output    = call_user_method(cipherObject, funcName TSRMLS_CC);
    zval_ptr_dtor(&funcName);
    zval_ptr_dtor(&output);
}
/* }}} */

/* {{{ proto void AuthenticatedSymmetricTransformationFilter::__sleep(void)
   Prevents serialization of a AuthenticatedSymmetricTransformationFilter instance */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricTransformationFilter, __sleep) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\AuthenticatedSymmetricTransformationFilter instances");
}
/* }}} */

/* {{{ proto void AuthenticatedSymmetricTransformationFilter::__wakeup(void)
   Prevents use of a AuthenticatedSymmetricTransformationFilter instance that has been unserialized */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricTransformationFilter, __wakeup) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\AuthenticatedSymmetricTransformationFilter instances");
}
/* }}} */

/* {{{ proto AuthenticatedSymmetricTransformationFilter::__construct(Cryptopp\AuthenticatedSymmetricCipherInterface cipher [, Cryptopp\PaddingInterface padding]) */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricTransformationFilter, __construct) {
    zval *cipherObject;
    zval *paddingObject = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O|O", &cipherObject, cryptopp_ce_SymmetricTransformationInterface, &paddingObject, cryptopp_ce_PaddingInterface)) {
        return;
    }

    // if no padding object, pick the default one
    bool createdPadding = false;

    if (NULL == paddingObject) {
        MAKE_STD_ZVAL(paddingObject);
        object_init_ex(paddingObject, cryptopp_ce_PaddingPkcs7);
        createdPadding = true;
    }

    // retrieve native cipher object
    CryptoPP::AuthenticatedSymmetricCipher *symmetricEncryptor;
    CryptoPP::AuthenticatedSymmetricCipher *symmetricDecryptor;
    bool parentConstructorError = false;
    bool cipherMustBeDestructed = false;

    if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_AuthenticatedSymmetricCipherAbstract TSRMLS_CC)) {
        // retrieve native mode objects
        symmetricEncryptor = getCryptoppAuthenticatedSymmetricCipherEncryptorPtr(cipherObject TSRMLS_CC);
        symmetricDecryptor = getCryptoppAuthenticatedSymmetricCipherDecryptorPtr(cipherObject TSRMLS_CC);

        if (NULL == symmetricEncryptor || NULL == symmetricDecryptor) {
            parentConstructorError = true;
        }
    } else {
        // create a proxy to the user php object
        try {
            symmetricEncryptor      = new AuthenticatedSymmetricCipherProxy::Encryption(cipherObject TSRMLS_CC);
            symmetricDecryptor      = new AuthenticatedSymmetricCipherProxy::Decryption(cipherObject TSRMLS_CC);
            cipherMustBeDestructed  = true;
        } catch (bool e) {
            return;
        } catch (const char *e) {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp internal error: AuthenticatedSymmetricTransformationFilter: %s", e);

            if (createdPadding) {
                zval_ptr_dtor(&paddingObject);
            }

            return;
        }
    }

    if (parentConstructorError) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(cipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : parent constructor was not called", ce->name);

        if (createdPadding) {
            zval_ptr_dtor(&paddingObject);
        }

        return;
    }

    // create native authenticated symmetric transformation filter
    AuthenticatedEncryptionFilter *stfEncryptor;
    AuthenticatedDecryptionFilter *stfDecryptor;

    try {
        stfEncryptor = new AuthenticatedEncryptionFilter(*symmetricEncryptor, paddingObject, cipherMustBeDestructed TSRMLS_CC);
        stfDecryptor = new AuthenticatedDecryptionFilter(*symmetricDecryptor, paddingObject, cipherMustBeDestructed TSRMLS_CC);
    } catch (std::exception &e) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp internal error: AuthenticatedSymmetricTransformationFilter: %s", e.what());

        if (createdPadding) {
            zval_ptr_dtor(&paddingObject);
        }

        return;
    }

    setCryptoppAuthenticatedSymmetricTransformationFilterEncryptorPtr(getThis(), stfEncryptor TSRMLS_CC);
    setCryptoppAuthenticatedSymmetricTransformationFilterDecryptorPtr(getThis(), stfDecryptor TSRMLS_CC);

    // hold the cipher mode object. if not, it can be deleted and associated encryptor/decryptor objects will be deleted too
    // same for padding
    zend_update_property(cryptopp_ce_AuthenticatedSymmetricTransformationFilter, getThis(), "cipher", 6, cipherObject TSRMLS_CC);
    zend_update_property(cryptopp_ce_AuthenticatedSymmetricTransformationFilter, getThis(), "padding", 7, paddingObject TSRMLS_CC);

    if (createdPadding) {
        zval_ptr_dtor(&paddingObject);
    }
}
/* }}} */

/* {{{ proto Cryptopp\AuthenticatedSymmetricCipherInterface AuthenticatedSymmetricTransformationFilter::getCipher(void)
       Returns the cipher object */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricTransformationFilter, getCipher) {
    zval *cipherObject;
    cipherObject = zend_read_property(cryptopp_ce_AuthenticatedSymmetricTransformationFilter, getThis(), "cipher", 6, 0 TSRMLS_CC);
    RETURN_ZVAL(cipherObject, 1, 0)
}
/* }}} */

/* {{{ proto Cryptopp\PaddingInterface AuthenticatedSymmetricTransformationFilter::getPadding(void)
       Returns the padding object */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricTransformationFilter, getPadding) {
    zval *paddingObject;
    paddingObject = zend_read_property(cryptopp_ce_AuthenticatedSymmetricTransformationFilter, getThis(), "padding", 7, 0 TSRMLS_CC);
    RETURN_ZVAL(paddingObject, 1, 0)
}
/* }}} */

// TODO method encrypt/decrypt: accept an input and output object (return boolean)

/* {{{ proto bool|string AuthenticatedSymmetricTransformationFilter::encryptString([string data, [string aad]])
       Encrypts and authenticate a string */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricTransformationFilter, encryptString) {
    char *data      = NULL;
    char *aad       = NULL;
    int dataSize    = 0;
    int aadSize     = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ss", &data, &dataSize, &aad, &aadSize)) {
        return;
    }

    AuthenticatedEncryptionFilter *stfEncryptor;
    stfEncryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_TRANSFORMATION_FILTER_GET_ENCRYPTOR_PTR(stfEncryptor)

    // if the cipher object is a native object, ensure that the key/iv is valid
    if (!isNativeCipherObjectValid(getThis() TSRMLS_CC)) {
        RETURN_FALSE
    }

    // ensure that at least one of data or aad is not null
    if (0 == dataSize && 0 == aadSize) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : either data or aad may be empty, not both", cryptopp_ce_AuthenticatedSymmetricTransformationFilter->name);
        RETURN_FALSE
    }

    // encrypt
    try {
        stfEncryptor->GetNextMessage();

        // add aad
        if (aadSize > 0) {
            stfEncryptor->ChannelPut(CryptoPP::AAD_CHANNEL, reinterpret_cast<byte*>(aad), aadSize);
            stfEncryptor->ChannelMessageEnd(CryptoPP::AAD_CHANNEL);
        }

        // encrypt data
        stfEncryptor->ChannelPut(CryptoPP::DEFAULT_CHANNEL, reinterpret_cast<byte*>(data), dataSize);
        stfEncryptor->ChannelMessageEnd(CryptoPP::DEFAULT_CHANNEL);

        // retrieve encrypted data and tag
        // the tag is right after encrypted data
        CryptoPP::lword retrievable = stfEncryptor->MaxRetrievable();
        restartCipherObject(getThis() TSRMLS_CC);

        if (retrievable > dataSize) {
            // return ciphertext
            byte ciphertext[retrievable];
            stfEncryptor->Get(ciphertext, retrievable);
            RETURN_STRINGL(reinterpret_cast<char*>(ciphertext), retrievable, 1)
        } else {
            // something goes wrong
            RETURN_FALSE
        }
    } catch (bool e) {
        RETURN_FALSE
    }
}
/* }}} */

/* {{{ proto bool|string AuthenticatedSymmetricTransformationFilter::decryptString([string ciphertext, [string aad]])
       Decrypts a string */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricTransformationFilter, decryptString) {
    char *ciphertext    = NULL;
    char *aad           = NULL;
    int ciphertextSize  = 0;
    int aadSize         = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ss", &ciphertext, &ciphertextSize, &aad, &aadSize)) {
        return;
    }

    AuthenticatedDecryptionFilter *stfDecryptor;
    stfDecryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_TRANSFORMATION_FILTER_GET_DECRYPTOR_PTR(stfDecryptor)

    // if the mode object is a native object, ensure that the key/iv is valid
    if (!isNativeCipherObjectValid(getThis() TSRMLS_CC)) {
        RETURN_FALSE
    }

    // ensure that at least one of ciphertext or aad is not null
    if (0 == ciphertextSize && 0 == aadSize) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : either ciphertext or aad may be empty, not both", cryptopp_ce_AuthenticatedSymmetricTransformationFilter->name);
        RETURN_FALSE
    }

    // decrypt
    try {
        stfDecryptor->GetNextMessage();

        // add aad
        if (aadSize > 0) {
            stfDecryptor->ChannelPut(CryptoPP::AAD_CHANNEL, reinterpret_cast<byte*>(aad), aadSize);
            stfDecryptor->ChannelMessageEnd(CryptoPP::AAD_CHANNEL);
        }

        // decrypt data
        stfDecryptor->ChannelPut(CryptoPP::DEFAULT_CHANNEL, reinterpret_cast<byte*>(ciphertext), ciphertextSize);
        stfDecryptor->ChannelMessageEnd(CryptoPP::DEFAULT_CHANNEL);

        CryptoPP::lword retrievable = stfDecryptor->MaxRetrievable();
        restartCipherObject(getThis() TSRMLS_CC);

        if (retrievable > 0 && retrievable <= ciphertextSize) {
            // return plain text
            byte plaintext[retrievable];
            stfDecryptor->Get(plaintext, retrievable);
            RETURN_STRINGL(reinterpret_cast<char*>(plaintext), retrievable, 1)
        } else {
            // something goes wrong
            RETURN_FALSE
        }
    } catch (bool e) {
        RETURN_FALSE
    } catch (CryptoPP::HashVerificationFilter::HashVerificationFailed &e) {
        // MAC not valid
        zend_throw_exception_ex(getCryptoppMacVerificationFailedException(), 0 TSRMLS_CC, (char*)"%s : MAC verification failed", cryptopp_ce_AuthenticatedSymmetricTransformationFilter->name);
        RETURN_FALSE
    }
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
