#ifndef PHP_AUTHENTICATED_SYMMETRIC_CIPHER_GENERIC_H
#define PHP_AUTHENTICATED_SYMMETRIC_CIPHER_GENERIC_H

#include "../../../php_cryptopp.h"

extern zend_class_entry *cryptopp_ce_AuthenticatedSymmetricCipherGeneric;
void init_class_AuthenticatedSymmetricCipherGeneric(TSRMLS_D);
bool isCryptoppAuthenticatedSymmetricCipherGenericMacKeyValid(zval *object, CryptoPP::AuthenticatedSymmetricCipher *cipher);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherGeneric, __construct);

/* {{{ methods declarations */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherGeneric, getCipher);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherGeneric, getMac);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherGeneric, setMacKey);
/* }}} */

/* {{{ AuthenticatedSymmetricCipher that take an instance of CryptoPP::SymmetricCipher and an instance of CryptoPP::MessageAuthenticationCode */
class AuthenticatedSymmetricCipherGeneric
{
public:
    /* {{{ base class */
    class Base : public CryptoPP::AuthenticatedSymmetricCipher, public SymmetricTransformationUserInterface
    {
    public:
        void ProcessData(byte *outString, const byte *inString, size_t length);
        void SetKeyWithIV(const byte *key, size_t length, const byte *iv, size_t ivLength);
        void SetKey(const byte *key, size_t length, const CryptoPP::NameValuePairs &params = CryptoPP::g_nullNameValuePairs);
        void Resynchronize(const byte *iv, int ivLength=-1);
        void SetMacKey(const byte *key, size_t length);
        bool IsValidKeyLength(size_t n) const;
        bool IsValidIvLength(size_t n);
        bool IsValidMacKeyLength(size_t n) const;
        void Restart();

        std::string AlgorithmName() const {return "generic";}

        unsigned int MandatoryBlockSize() const {return m_cipher->MandatoryBlockSize();}
        unsigned int OptimalBlockSize() const  {return m_cipher->OptimalBlockSize();}
        unsigned int MinLastBlockSize() const {return m_cipher->MinLastBlockSize();}
        unsigned int GetOptimalBlockSizeUsed() const {return m_cipher->GetOptimalBlockSizeUsed();}
        unsigned int OptimalDataAlignment() const {return m_cipher->OptimalDataAlignment();}
        void ProcessLastBlock(byte *outString, const byte *inString, size_t length) {m_cipher->ProcessLastBlock(outString, inString, length);}
        bool IsRandomAccess() const {return m_cipher->IsRandomAccess();}
        void Seek(CryptoPP::lword n) {m_cipher->Seek(n);}
        bool IsSelfInverting() const {return m_cipher->IsSelfInverting();}
        size_t MinKeyLength() const {return m_cipher->MinKeyLength();}
        size_t MaxKeyLength() const {return m_cipher->MaxKeyLength();}
        size_t DefaultKeyLength() const {return m_cipher->DefaultKeyLength();}
        size_t GetValidKeyLength(size_t n) const {return m_cipher->GetValidKeyLength(n);}
        unsigned int IVSize() const {return m_cipher->IVSize();}
        unsigned int MinIVLength() const {return m_cipher->MinIVLength();}
        unsigned int MaxIVLength() const {return m_cipher->MaxIVLength();}
        IV_Requirement IVRequirement() const {return m_cipher->IVRequirement();}
        void GetNextIV(CryptoPP::RandomNumberGenerator &rng, byte *IV) {m_cipher->GetNextIV(rng, IV);}

        unsigned int DigestSize() const {return m_mac->DigestSize();}
        void Update(const byte *input, size_t length) {m_mac->Update(input, length);}
        void Final(byte *digest) {TruncatedFinal(digest, DigestSize());}
        void TruncatedFinal(byte *digest, size_t digestSize) {m_mac->TruncatedFinal(digest, digestSize);}
        void CalculateDigest(byte *digest, const byte *input, size_t length) {Update(input, length); Final(digest);}
        bool Verify(const byte *digest) {return TruncatedVerify(digest, DigestSize());}
        bool VerifyDigest(const byte *digest, const byte *input, size_t length) {Update(input, length); return Verify(digest);}
        void CalculateTruncatedDigest(byte *digest, size_t digestSize, const byte *input, size_t length) {Update(input, length); TruncatedFinal(digest, digestSize);}
        bool VerifyTruncatedDigest(const byte *digest, size_t digestLength, const byte *input, size_t length) {Update(input, length); return TruncatedVerify(digest, digestLength);}
        byte * CreateUpdateSpace(size_t &size) {return m_mac->CreateUpdateSpace(size);}

        // unused
        CryptoPP::lword MaxHeaderLength() const {return 0;}
        CryptoPP::lword MaxMessageLength() const {return 0;}
        CryptoPP::lword MaxFooterLength() const {return 0;}
        bool NeedsPrespecifiedDataLengths() const {return false;}
        void EncryptAndAuthenticate(byte *ciphertext, byte *mac, size_t macSize, const byte *iv, int ivLength, const byte *header, size_t headerLength, const byte *message, size_t messageLength) {}
        bool DecryptAndVerify(byte *message, const byte *mac, size_t macLength, const byte *iv, int ivLength, const byte *header, size_t headerLength, const byte *ciphertext, size_t ciphertextLength) {return true;}

    protected:
        Base(zval *zCipher, zval *zMac, CryptoPP::SymmetricCipher *cipher, CryptoPP::MessageAuthenticationCode *mac);

        // unused
        const Algorithm & GetAlgorithm() const {return *static_cast<const CryptoPP::MessageAuthenticationCode *>(this);}
        void UncheckedSpecifyDataLengths(CryptoPP::lword headerLength, CryptoPP::lword messageLength, CryptoPP::lword footerLength) {}
        void UncheckedSetKey(const byte *key, unsigned int length, const CryptoPP::NameValuePairs &params) {}

        zval *m_zCipher;
        zval *m_zMac;
        zval *m_funcnameRestart;
        CryptoPP::SymmetricCipher *m_cipher;
        CryptoPP::MessageAuthenticationCode *m_mac;
    };
    /* }}} */

    /* {{{ encryption class */
    class Encryption : public Base
    {
    public:
        Encryption(zval *zCipher, zval *zMac, CryptoPP::SymmetricCipher *cipher, CryptoPP::MessageAuthenticationCode *mac);
        bool IsForwardTransformation() const {return true;};
    };
    /* }}} */

    /* {{{ decryption class */
    class Decryption : public Base
    {
    public:
        Decryption(zval *zCipher, zval *zMac, CryptoPP::SymmetricCipher *cipher, CryptoPP::MessageAuthenticationCode *mac);
        bool IsForwardTransformation() const {return false;};
    };
    /* }}} */
};
/* }}} */

#endif /* PHP_AUTHENTICATED_SYMMETRIC_CIPHER_GENERIC_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
