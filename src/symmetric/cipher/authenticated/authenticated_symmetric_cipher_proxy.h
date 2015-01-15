#ifndef PHP_AUTHENTICATED_SYMMETRIC_CIPHER_PROXY_H
#define PHP_AUTHENTICATED_SYMMETRIC_CIPHER_PROXY_H

#include "src/php_cryptopp.h"
#include <string>

class AuthenticatedSymmetricCipherProxy
{
public:
    /* {{{ base class */
    class Base : public CryptoPP::AuthenticatedSymmetricCipher
    {
    public:
        ~Base();

        unsigned int MandatoryBlockSize() const;
        unsigned int OptimalBlockSize() const;
        void ProcessData(byte *outString, const byte *inString, size_t length);
        unsigned int MinLastBlockSize() const;
        void Update(const byte *input, size_t length);
        void TruncatedFinal(byte *digest, size_t digestSize);
        unsigned int DigestSize() const;
        void Resynchronize(const byte *iv, int ivLength = -1);
        void Restart();

        std::string AlgorithmName() const {return m_name;};

        // unused
        CryptoPP::lword MaxHeaderLength() const {return 0;}
        CryptoPP::lword MaxMessageLength() const {return 0;}
        CryptoPP::lword MaxFooterLength() const {return 0;}
        bool NeedsPrespecifiedDataLengths() const {return false;}
        void EncryptAndAuthenticate(byte *ciphertext, byte *mac, size_t macSize, const byte *iv, int ivLength, const byte *header, size_t headerLength, const byte *message, size_t messageLength) {}
        bool DecryptAndVerify(byte *message, const byte *mac, size_t macLength, const byte *iv, int ivLength, const byte *header, size_t headerLength, const byte *ciphertext, size_t ciphertextLength) {return true;}
        unsigned int GetOptimalBlockSizeUsed() const {return 0;}
        unsigned int OptimalDataAlignment() const {return 1;}
        void ProcessLastBlock(byte *outString, const byte *inString, size_t length) {}
        bool IsRandomAccess() const {return false;}
        void Seek(CryptoPP::lword n) {assert(!IsRandomAccess()); throw CryptoPP::NotImplemented("StreamTransformation: this object doesn't support random access");}
        bool IsSelfInverting() const {return false;}
        size_t MinKeyLength() const {return 8;}
        size_t MaxKeyLength() const {return 8;}
        size_t DefaultKeyLength() const {return 8;}
        size_t GetValidKeyLength(size_t n) const {return 8;}
        void SetKey(const byte *key, size_t length, const CryptoPP::NameValuePairs &params = CryptoPP::g_nullNameValuePairs) {}
        IV_Requirement IVRequirement() const {return RANDOM_IV;}
        void GetNextIV(CryptoPP::RandomNumberGenerator &rng, byte *IV) {}
        byte * CreateUpdateSpace(size_t &size) {return NULL;}
        void Final(byte *digest) {TruncatedFinal(digest, DigestSize());}
        void CalculateDigest(byte *digest, const byte *input, size_t length) {Update(input, length); Final(digest);}
        bool Verify(const byte *digest) {return TruncatedVerify(digest, DigestSize());}
        bool VerifyDigest(const byte *digest, const byte *input, size_t length) {Update(input, length); return Verify(digest);}
        void CalculateTruncatedDigest(byte *digest, size_t digestSize, const byte *input, size_t length) {Update(input, length); TruncatedFinal(digest, digestSize);}
        bool VerifyTruncatedDigest(const byte *digest, size_t digestLength, const byte *input, size_t length) {Update(input, length); return TruncatedVerify(digest, digestLength);}

    protected:
        Base(zval *authenticatedCipherObject, const char* processDataFuncname, const char* updateFuncname, const char* finalizeFuncname TSRMLS_DC);

        // unused
        const Algorithm & GetAlgorithm() const {return *static_cast<const CryptoPP::MessageAuthenticationCode *>(this);}
        void UncheckedSpecifyDataLengths(CryptoPP::lword headerLength, CryptoPP::lword messageLength, CryptoPP::lword footerLength) {}
        void UncheckedSetKey(const byte *key, unsigned int length, const CryptoPP::NameValuePairs &params) {}

        zval *m_authenticatedCipherObject;
        std::string m_name;
        size_t m_blockSize;
        size_t m_digestSize;
        zval *m_funcnameProcessData;
        zval *m_funcnameUpdate;
        zval *m_funcnameFinalize;
        zval *m_funcnameRestart;
        M_TSRMLS_D;
    };
    /* }}} */

    /* {{{ encryption class */
    class Encryption : public Base
    {
    public:
        Encryption(zval *authenticatedCipherObject TSRMLS_DC) : Base(authenticatedCipherObject, "encrypt", "addEncryptionAdditionalData", "finalizeEncryption" TSRMLS_CC) {};
        bool IsForwardTransformation() const {return true;};
    };
    /* }}} */

    /* {{{ decryption class */
    class Decryption : public Base
    {
    public:
        Decryption(zval *authenticatedCipherObject TSRMLS_DC) : Base(authenticatedCipherObject, "decrypt", "addDecryptionAdditionalData", "finalizeDecryption" TSRMLS_CC) {};
        bool IsForwardTransformation() const {return false;};
    };
    /* }}} */
};

#endif /* PHP_AUTHENTICATED_SYMMETRIC_CIPHER_PROXY_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
