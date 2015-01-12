#ifndef PHP_MAC_PROXY_H
#define PHP_MAC_PROXY_H

#include "../php_cryptopp.h"

class MacProxy : public CryptoPP::MessageAuthenticationCode
{
public:
    MacProxy(zval *hashObject TSRMLS_DC);
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
    bool IsValidKeyLength(size_t n) const;
    bool IsValidKeyLength(size_t n);

    static const char * CRYPTOPP_API StaticAlgorithmName() {return "User";}

    // unused
    size_t MinKeyLength() const {return 1;}
    size_t MaxKeyLength() const {return 1;}
    size_t DefaultKeyLength() const {return 1;}
    size_t GetValidKeyLength(size_t n) const {return 1;}
    IV_Requirement IVRequirement() const {return NOT_RESYNCHRONIZABLE;}
    unsigned int OptimalDataAlignment() const {return 1;}
    byte * CreateUpdateSpace(size_t &size) {return NULL;}
    bool Verify(const byte *digest) {return true;}
    bool VerifyDigest(const byte *digest, const byte *input, size_t length) {return true;}
    void CalculateTruncatedDigest(byte *digest, size_t digestSize, const byte *input, size_t length) {}
    bool TruncatedVerify(const byte *digest, size_t digestLength) {return true;}
    bool VerifyTruncatedDigest(const byte *digest, size_t digestLength, const byte *input, size_t length) {return true;}

protected:
    // unused
    void UncheckedSetKey(const byte *key, unsigned int length, const CryptoPP::NameValuePairs &params) {};

    zval *m_macObject;
    unsigned int m_blockSize;
    unsigned int m_digestSize;
    zval *m_funcnameCalculateDigest;
    zval *m_funcnameUpdate;
    zval *m_funcnameFinal;
    zval *m_funcnameRestart;
    zval *m_funcnameSetKey;
    zval *m_funcnameIsValidKeyLength;
    M_TSRMLS_D;
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
