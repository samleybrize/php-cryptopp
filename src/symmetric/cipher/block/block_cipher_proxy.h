#ifndef PHP_BLOCK_CIPHER_PROXY_H
#define PHP_BLOCK_CIPHER_PROXY_H

#include "../../../php_cryptopp.h"
#include "php_block_cipher_interface.h"

class BlockCipherProxy
{
public:
    /* {{{ base class */
    class Base : public CryptoPP::BlockCipher
    {
    public:
        ~Base();

        unsigned int BlockSize() const;
        bool IsValidKeyLength(size_t n) const;
        bool IsValidKeyLength(size_t n);
        void SetKey(const byte *key, size_t length, const CryptoPP::NameValuePairs &params = CryptoPP::g_nullNameValuePairs);
        void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const;

        // TODO todel
//        void ProcessBlock(const byte *inBlock, byte *outBlock) const {php_printf("ProcessBlock1\n"); CryptoPP::AES::Encryption::ProcessBlock(inBlock, outBlock);};
//        void ProcessBlock(byte *inoutBlock) const {php_printf("ProcessBlock2\n"); CryptoPP::AES::Encryption::ProcessBlock(inoutBlock);};
//        size_t AdvancedProcessBlocks(const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, size_t length, CryptoPP::word32 flags) const;

        // TODO unused
        size_t GetValidKeyLength(size_t n) const {php_printf("GetValidKeyLength\n"); return 1;}
        unsigned int OptimalDataAlignment() const {php_printf("OptimalDataAlignment\n"); return 1;}
        bool IsPermutation() const {php_printf("IsPermutation\n"); return true;}
        unsigned int OptimalNumberOfParallelBlocks() const {php_printf("OptimalNumberOfParallelBlocks\n"); return 1;}
        std::string AlgorithmName() const {php_printf("AlgorithmName\n"); return "";} // TODO
        size_t MinKeyLength() const {php_printf("MinKeyLength\n"); return 1;}
        size_t MaxKeyLength() const {php_printf("MaxKeyLength\n"); return 1;}
        size_t DefaultKeyLength() const {php_printf("DefaultKeyLength\n"); return 1;}
        IV_Requirement IVRequirement() const {php_printf("IVRequirement\n"); return NOT_RESYNCHRONIZABLE;}
        unsigned int IVSize() const {php_printf("IVSize\n"); return 0;}
        unsigned int MinIVLength() const {php_printf("MinIVLength\n"); return 0;}
        unsigned int MaxIVLength() const {php_printf("MaxIVLength\n"); return 0;}
        void Resynchronize(const byte *iv, int ivLength=-1) {php_printf("Resynchronize\n");}
        void GetNextIV(CryptoPP::RandomNumberGenerator &rng, byte *IV) {php_printf("GetNextIV\n");}
//        void SetKeyWithRounds(const byte *key, size_t length, int rounds) {php_printf("SetKeyWithRounds\n");}
//        void SetKeyWithIV(const byte *key, size_t length, const byte *iv, size_t ivLength) {php_printf("SetKeyWithIV1\n");}
//        void SetKeyWithIV(const byte *key, size_t length, const byte *iv) {php_printf("SetKeyWithIV2\n");}
//        bool IsResynchronizable() const {php_printf("IsResynchronizable\n"); return false;}
//        bool CanUseRandomIVs() const {php_printf("CanUseRandomIVs\n"); return false;}
//        bool CanUsePredictableIVs() const {php_printf("CanUsePredictableIVs\n"); return false;}
//        bool CanUseStructuredIVs() const {php_printf("CanUseStructuredIVs\n"); return false;}
//        unsigned int DefaultIVLength() const {php_printf("DefaultIVLength\n"); return 0;}

    protected:
        Base(zval *blockCipherObject, const char* processDataFuncname, const char *processBlockFuncname);

        // TODO unused
        void UncheckedSetKey(const byte *key, unsigned int length, const CryptoPP::NameValuePairs &params) {php_printf("UncheckedSetKey\n");};
//        const Algorithm & GetAlgorithm() const {php_printf("GetAlgorithm\n"); return this;};
//        void ThrowIfInvalidKeyLength(size_t length) {php_printf("ThrowIfInvalidKeyLength\n");};
//        void ThrowIfResynchronizable() {php_printf("ThrowIfResynchronizable\n");};
//        void ThrowIfInvalidIV(const byte *iv) {php_printf("ThrowIfInvalidIV\n");};
//        size_t ThrowIfInvalidIVLength(int size) {php_printf("ThrowIfInvalidIVLength\n"); return 0;};
//        const byte * GetIVAndThrowIfInvalid(const CryptoPP::NameValuePairs &params, size_t &size) {php_printf("GetIVAndThrowIfInvalid\n");};
//        inline void AssertValidKeyLength(size_t length) const {php_printf("AssertValidKeyLength\n");};

        size_t m_blockSize;
        zval *m_blockCipherObject;
        zval *m_funcnameProcessData;
        zval *m_funcnameProcessBlock;
        zval *m_funcnameIsValidKeyLength;
        zval *m_funcnameSetKey;
    };
    /* }}} */

    /* {{{ encryption class */
    class Encryption : public Base
    {
    public:
        Encryption(zval *blockCipherObject) : Base(blockCipherObject, "encryptData", "encryptBlock"){};
        bool IsForwardTransformation() const {return true;};
        inline CryptoPP::CipherDir GetCipherDirection() const {return CryptoPP::ENCRYPTION;}
    };
    /* }}} */

    /* {{{ decryption class */
    class Decryption : public Base
    {
    public:
        Decryption(zval *blockCipherObject) : Base(blockCipherObject, "decryptData", "decryptBlock"){};
        bool IsForwardTransformation() const {return false;};
        inline CryptoPP::CipherDir GetCipherDirection() const {return CryptoPP::DECRYPTION;}
    };
    /* }}} */
};

#endif /* PHP_BLOCK_CIPHER_PROXY_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
