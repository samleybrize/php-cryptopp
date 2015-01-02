#ifndef PHP_AUTHENTICATED_SYMMETRIC_CIPHER_GENERIC_H
#define PHP_AUTHENTICATED_SYMMETRIC_CIPHER_GENERIC_H

#include "../../../php_cryptopp.h"

void init_class_AuthenticatedSymmetricCipherGeneric(TSRMLS_D);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherGeneric, __construct);

// TODO
class AuthenticatedSymmetricCipherGeneric
{
public:
    /* {{{ base class */
    class Base : public CryptoPP::AuthenticatedSymmetricCipher
    {
    public:
        unsigned int MandatoryBlockSize() const;
        unsigned int OptimalBlockSize() const;
        void ProcessData(byte *outString, const byte *inString, size_t length);
        unsigned int MinLastBlockSize() const;
        void Update(const byte *input, size_t length);
        void TruncatedFinal(byte *digest, size_t digestSize);
        unsigned int DigestSize() const;
        std::string AlgorithmName() const {return "generic";};
        void Restart(const byte *iv, int ivLength);

        unsigned int GetOptimalBlockSizeUsed() const {return m_cipher->GetOptimalBlockSizeUsed();}
        unsigned int OptimalDataAlignment() const {return m_cipher->OptimalDataAlignment();}
        void ProcessLastBlock(byte *outString, const byte *inString, size_t length) {m_cipher->ProcessLastBlock(outString, inString, length);}
        bool IsRandomAccess() const {return m_cipher->IsRandomAccess();}
        void Seek(CryptoPP::lword n) {m_cipher->Seek(n);}
        bool IsSelfInverting() const {return m_cipher->IsSelfInverting();}
        size_t MinKeyLength() const {return m_cipher->MinKeyLength();} // TODO
        size_t MaxKeyLength() const {return m_cipher->MaxKeyLength();} // TODO
        size_t DefaultKeyLength() const {return m_cipher->DefaultKeyLength();} // TODO
        size_t GetValidKeyLength(size_t n) const {return m_cipher->GetValidKeyLength(n);} // TODO
        void SetKey(const byte *key, size_t length, const CryptoPP::NameValuePairs &params = CryptoPP::g_nullNameValuePairs) {m_cipher->SetKey(key, length, params);} // TODO
        IV_Requirement IVRequirement() const {return m_cipher->IVRequirement();}
        void Resynchronize(const byte *iv, int ivLength=-1) {m_cipher->Resynchronize(iv, ivLength); m_mac->Restart();};
        void GetNextIV(CryptoPP::RandomNumberGenerator &rng, byte *IV) {m_cipher->GetNextIV(rng, IV);}

        void Final(byte *digest) {TruncatedFinal(digest, DigestSize());}
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
        Base(CryptoPP::SymmetricCipher *cipher, CryptoPP::MessageAuthenticationCode *mac);

        // unused
        const Algorithm & GetAlgorithm() const {return *static_cast<const CryptoPP::MessageAuthenticationCode *>(this);}
        void UncheckedSpecifyDataLengths(CryptoPP::lword headerLength, CryptoPP::lword messageLength, CryptoPP::lword footerLength) {}
        void UncheckedSetKey(const byte *key, unsigned int length, const CryptoPP::NameValuePairs &params) {}

        CryptoPP::SymmetricCipher *m_cipher;
        CryptoPP::MessageAuthenticationCode *m_mac;
    };
    /* }}} */

    /* {{{ encryption class */
    class Encryption : public Base
    {
    public:
        Encryption(CryptoPP::SymmetricCipher *cipher, CryptoPP::MessageAuthenticationCode *mac);
        bool IsForwardTransformation() const {return true;};
    };
    /* }}} */

    /* {{{ decryption class */
    class Decryption : public Base
    {
    public:
        Decryption(CryptoPP::SymmetricCipher *cipher, CryptoPP::MessageAuthenticationCode *mac);
        bool IsForwardTransformation() const {return false;};
    };
    /* }}} */
};

#endif /* PHP_AUTHENTICATED_SYMMETRIC_CIPHER_GENERIC_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
