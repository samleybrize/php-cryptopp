/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_AUTHENTICATED_SYMMETRIC_TRANSFORMATION_FILTER_H
#define PHP_AUTHENTICATED_SYMMETRIC_TRANSFORMATION_FILTER_H

#include "src/php_cryptopp.h"
#include "php_symmetric_transformation_filter.h"
#include <filters.h>
#include <string>

void init_class_AuthenticatedSymmetricTransformationFilter(TSRMLS_D);

/* {{{ fork of CryptoPP::AuthenticatedEncryptionFilter to support padding schemes as objects */
class AuthenticatedEncryptionFilter : public SymmetricTransformationFilter
{
public:
    /*! See StreamTransformationFilter for documentation on BlockPaddingScheme  */
    AuthenticatedEncryptionFilter(CryptoPP::AuthenticatedSymmetricCipher &c, zval *paddingObject, bool cipherMustBeDestructed TSRMLS_DC);

    void IsolatedInitialize(const CryptoPP::NameValuePairs &parameters);
    byte * ChannelCreatePutSpace(const std::string &channel, size_t &size);
    size_t ChannelPut2(const std::string &channel, const byte *begin, size_t length, int messageEnd, bool blocking);
    void LastPut(const byte *inString, size_t length);

protected:
    CryptoPP::HashFilter m_hf;
};
/* }}} */

/* {{{ fork of CryptoPP::HashVerificationFilter to return some protected properties */
class HashVerificationFilter : public CryptoPP::HashVerificationFilter
{
public:
    HashVerificationFilter(CryptoPP::HashTransformation &hm, CryptoPP::BufferedTransformation *attachment = NULL, CryptoPP::word32 flags = DEFAULT_FLAGS, int truncatedDigestSize = -1) :
        CryptoPP::HashVerificationFilter(hm, attachment, flags, truncatedDigestSize) {}
    size_t FirstSize();
    size_t LastSize();
};
/* }}} */

/* {{{ fork of CryptoPP::AuthenticatedDecryptionFilter to support padding schemes as objects */
class AuthenticatedDecryptionFilter : public CryptoPP::FilterWithBufferedInput
{
public:
    enum Flags {MAC_AT_END=0, MAC_AT_BEGIN=1, THROW_EXCEPTION=16, DEFAULT_FLAGS = THROW_EXCEPTION};

    /*! See StreamTransformationFilter for documentation on BlockPaddingScheme  */
    AuthenticatedDecryptionFilter(CryptoPP::AuthenticatedSymmetricCipher &c, zval *paddingObject, bool cipherMustBeDestructed TSRMLS_DC, CryptoPP::word32 flags = DEFAULT_FLAGS);

    std::string AlgorithmName() const {return m_hashVerifier.AlgorithmName();}
    byte * ChannelCreatePutSpace(const std::string &channel, size_t &size);
    size_t ChannelPut2(const std::string &channel, const byte *begin, size_t length, int messageEnd, bool blocking);
    bool GetLastResult() const {return m_hashVerifier.GetLastResult();}

protected:
    void InitializeDerivedAndReturnNewSizes(const CryptoPP::NameValuePairs &parameters, size_t &firstSize, size_t &blockSize, size_t &lastSize);
    void FirstPut(const byte *inString);
    void NextPutMultiple(const byte *inString, size_t length);
    void LastPut(const byte *inString, size_t length);

    HashVerificationFilter m_hashVerifier;
    SymmetricTransformationFilter m_streamFilter;
};
/* }}} */

/* {{{ get the pointer to the native stf encryptor object of the php class */
#define CRYPTOPP_AUTHENTICATED_SYMMETRIC_TRANSFORMATION_FILTER_GET_ENCRYPTOR_PTR(ptrName) \
    ptrName = getCryptoppAuthenticatedSymmetricTransformationFilterEncryptorPtr(getThis() TSRMLS_CC); \
                                                                            \
    if (NULL == ptrName) {                                                  \
        RETURN_FALSE;                                                       \
    }                                                                       \
/* }}} */

/* {{{ get the pointer to the native stf decryptor object of the php class */
#define CRYPTOPP_AUTHENTICATED_SYMMETRIC_TRANSFORMATION_FILTER_GET_DECRYPTOR_PTR(ptrName) \
    ptrName = getCryptoppAuthenticatedSymmetricTransformationFilterDecryptorPtr(getThis() TSRMLS_CC); \
                                                                            \
    if (NULL == ptrName) {                                                  \
        RETURN_FALSE;                                                       \
    }                                                                       \
/* }}} */

/* {{{ object creation related stuff */
struct AuthenticatedSymmetricTransformationFilterContainer {
    zend_object std;
    AuthenticatedEncryptionFilter *stfEncryptor;
    AuthenticatedDecryptionFilter *stfDecryptor;
};

extern zend_object_handlers AuthenticatedSymmetricTransformationFilter_object_handlers;
void AuthenticatedSymmetricTransformationFilter_free_storage(void *object TSRMLS_DC);
/* }}} */

/* {{{ methods declarations */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricTransformationFilter, __construct);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricTransformationFilter, __sleep);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricTransformationFilter, __wakeup);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricTransformationFilter, getCipher);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricTransformationFilter, getPadding);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricTransformationFilter, encryptString);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricTransformationFilter, decryptString);
/* }}} */

#endif /* PHP_AUTHENTICATED_SYMMETRIC_TRANSFORMATION_FILTER_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
