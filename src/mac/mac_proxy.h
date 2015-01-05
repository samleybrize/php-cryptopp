#ifndef PHP_MAC_PROXY_H
#define PHP_MAC_PROXY_H

#include "../php_cryptopp.h"

class MacProxy : public CryptoPP::MessageAuthenticationCode
{
public:
    MacProxy(zval *hashObject);
    ~MacProxy();
    unsigned int DigestSize() const;
    unsigned int BlockSize() const;
    void Update(const byte *input, size_t length);
    virtual void Final(byte *digest);
    void TruncatedFinal(byte *digest, size_t digestSize);
    void Restart();
    void CalculateDigest(byte *digest, const byte *input, size_t length);
    unsigned int OptimalBlockSize() const;
    void SetKey(const byte *key, size_t length, const CryptoPP::NameValuePairs &params = CryptoPP::g_nullNameValuePairs);
    bool IsValidKeyLength(size_t n) const {return n == GetValidKeyLength(n);} // TODO

    static const char * CRYPTOPP_API StaticAlgorithmName() {return "User";}

    // TODO
//    size_t MinKeyLength() const =0;
//    size_t MaxKeyLength() const =0;
//    size_t DefaultKeyLength() const =0;
//    size_t GetValidKeyLength(size_t n) const =0;
//    void SetKeyWithRounds(const byte *key, size_t length, int rounds);
//    void SetKeyWithIV(const byte *key, size_t length, const byte *iv, size_t ivLength);
//    void SetKeyWithIV(const byte *key, size_t length, const byte *iv) {SetKeyWithIV(key, length, iv, IVSize());}
//    IV_Requirement IVRequirement() const =0;
//    bool IsResynchronizable() const {return IVRequirement() < NOT_RESYNCHRONIZABLE;}
//    bool CanUseRandomIVs() const {return IVRequirement() <= UNPREDICTABLE_RANDOM_IV;}
//    bool CanUsePredictableIVs() const {return IVRequirement() <= RANDOM_IV;}
//    bool CanUseStructuredIVs() const {return IVRequirement() <= UNIQUE_IV;}
//    unsigned int IVSize() const {throw CryptoPP::NotImplemented(GetAlgorithm().AlgorithmName() + ": this object doesn't support resynchronization");}
//    unsigned int DefaultIVLength() const {return IVSize();}
//    unsigned int MinIVLength() const {return IVSize();}
//    unsigned int MaxIVLength() const {return IVSize();}
//    void Resynchronize(const byte *iv, int ivLength=-1) {throw CryptoPP::NotImplemented(GetAlgorithm().AlgorithmName() + ": this object doesn't support resynchronization");}
//    void GetNextIV(CryptoPP::RandomNumberGenerator &rng, byte *IV);

    // unused
    unsigned int OptimalDataAlignment() const {return 1;}
    byte * CreateUpdateSpace(size_t &size) {return NULL;}
    bool Verify(const byte *digest) {return true;}
    bool VerifyDigest(const byte *digest, const byte *input, size_t length) {return true;}
    void CalculateTruncatedDigest(byte *digest, size_t digestSize, const byte *input, size_t length) {}
    bool TruncatedVerify(const byte *digest, size_t digestLength) {return true;}
    bool VerifyTruncatedDigest(const byte *digest, size_t digestLength, const byte *input, size_t length) {return true;}

protected:
    zval *m_macObject;
    unsigned int m_blockSize;
    unsigned int m_digestSize;
    zval *m_funcnameCalculateDigest;
    zval *m_funcnameUpdate;
    zval *m_funcnameFinal;
    zval *m_funcnameRestart;
    zval *m_funcnameSetKey;
};

#endif /* PHP_MAC_PROXY_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
