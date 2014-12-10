#ifndef PHP_BLOCK_CIPHER_PROXY_H
#define PHP_BLOCK_CIPHER_PROXY_H

#include "../../../php_cryptopp.h"
#include "php_block_cipher_interface.h"

#include <aes.h> // TODO

class BlockCipherProxy
{
public:
    /* {{{ base class */
    class Base : public CryptoPP::AES::Encryption
    {
    public:
        ~Base();

        unsigned int BlockSize() const;
        bool IsValidKeyLength(size_t n) const;
        bool IsValidKeyLength(size_t n);
        void SetKey(const byte *key, size_t length, const CryptoPP::NameValuePairs &params = CryptoPP::g_nullNameValuePairs);

        // TODO
        size_t AdvancedProcessBlocks(const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, size_t length, CryptoPP::word32 flags) const {php_printf("AdvancedProcessBlocks\n"); return CryptoPP::AES::Encryption::AdvancedProcessBlocks(inBlocks, xorBlocks, outBlocks, length, flags);};

        // TODO unused
        size_t GetValidKeyLength(size_t n) const {php_printf("GetValidKeyLength\n"); return CryptoPP::AES::Encryption::GetValidKeyLength(n);};
        void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const {php_printf("ProcessAndXorBlock\n"); CryptoPP::AES::Encryption::ProcessAndXorBlock(inBlock, xorBlock, outBlock);};
        void ProcessBlock(const byte *inBlock, byte *outBlock) const {php_printf("ProcessBlock1\n"); CryptoPP::AES::Encryption::ProcessBlock(inBlock, outBlock);};
        void ProcessBlock(byte *inoutBlock) const {php_printf("ProcessBlock2\n"); CryptoPP::AES::Encryption::ProcessBlock(inoutBlock);};
        unsigned int OptimalDataAlignment() const {php_printf("OptimalDataAlignment\n"); return CryptoPP::AES::Encryption::OptimalDataAlignment();};
        bool IsPermutation() const {php_printf("IsPermutation\n"); return CryptoPP::AES::Encryption::IsPermutation();};
        unsigned int OptimalNumberOfParallelBlocks() const {php_printf("OptimalNumberOfParallelBlocks\n"); return CryptoPP::AES::Encryption::OptimalNumberOfParallelBlocks();};
        inline CryptoPP::CipherDir GetCipherDirection() const {php_printf("GetCipherDirection\n"); return CryptoPP::AES::Encryption::GetCipherDirection();};
        std::string AlgorithmName() const {php_printf("AlgorithmName\n"); return CryptoPP::AES::Encryption::AlgorithmName();};
        size_t MinKeyLength() const {php_printf("MinKeyLength\n"); return CryptoPP::AES::Encryption::MinKeyLength();};
        size_t MaxKeyLength() const {php_printf("MaxKeyLength\n"); return CryptoPP::AES::Encryption::MaxKeyLength();};
        size_t DefaultKeyLength() const {php_printf("DefaultKeyLength\n"); return CryptoPP::AES::Encryption::DefaultKeyLength();};
        void SetKeyWithRounds(const byte *key, size_t length, int rounds) {php_printf("SetKeyWithRounds\n"); CryptoPP::AES::Encryption::SetKeyWithRounds(key, length, rounds);};
        void SetKeyWithIV(const byte *key, size_t length, const byte *iv, size_t ivLength) {php_printf("SetKeyWithIV1\n"); CryptoPP::AES::Encryption::SetKeyWithIV(key, length, iv, ivLength);};
        void SetKeyWithIV(const byte *key, size_t length, const byte *iv) {php_printf("SetKeyWithIV2\n"); CryptoPP::AES::Encryption::SetKeyWithIV(key, length, iv);};
        IV_Requirement IVRequirement() const {php_printf("IVRequirement\n"); return CryptoPP::AES::Encryption::IVRequirement();};
        bool IsResynchronizable() const {php_printf("IsResynchronizable\n"); return CryptoPP::AES::Encryption::IsResynchronizable();};
        bool CanUseRandomIVs() const {php_printf("CanUseRandomIVs\n"); return CryptoPP::AES::Encryption::CanUseRandomIVs();};
        bool CanUsePredictableIVs() const {php_printf("CanUsePredictableIVs\n"); return CryptoPP::AES::Encryption::CanUsePredictableIVs();};
        bool CanUseStructuredIVs() const {php_printf("CanUseStructuredIVs\n"); return CryptoPP::AES::Encryption::CanUseStructuredIVs();};
        unsigned int IVSize() const {php_printf("IVSize\n"); return CryptoPP::AES::Encryption::IVSize();};
        unsigned int DefaultIVLength() const {php_printf("DefaultIVLength\n"); return CryptoPP::AES::Encryption::DefaultIVLength();};
        unsigned int MinIVLength() const {php_printf("MinIVLength\n"); return CryptoPP::AES::Encryption::MinIVLength();};
        unsigned int MaxIVLength() const {php_printf("MaxIVLength\n"); return CryptoPP::AES::Encryption::MaxIVLength();};
        void Resynchronize(const byte *iv, int ivLength=-1) {php_printf("Resynchronize\n"); CryptoPP::AES::Encryption::Resynchronize(iv, ivLength);};
        void GetNextIV(CryptoPP::RandomNumberGenerator &rng, byte *IV) {php_printf("GetNextIV\n"); CryptoPP::AES::Encryption::GetNextIV(rng, IV);};

    protected:
        Base(zval *blockCipherObject, const char* processDataFuncname);

        // TODO unused
        void UncheckedSetKey(const byte *key, unsigned int length, const CryptoPP::NameValuePairs &params) {php_printf("UncheckedSetKey\n"); CryptoPP::AES::Encryption::UncheckedSetKey(key, length, params);};
        const Algorithm & GetAlgorithm() const {php_printf("GetAlgorithm\n"); return CryptoPP::AES::Encryption::GetAlgorithm();};
        void ThrowIfInvalidKeyLength(size_t length) {php_printf("ThrowIfInvalidKeyLength\n"); CryptoPP::AES::Encryption::ThrowIfInvalidKeyLength(length);};
        void ThrowIfResynchronizable() {php_printf("ThrowIfResynchronizable\n"); CryptoPP::AES::Encryption::ThrowIfResynchronizable();};
        void ThrowIfInvalidIV(const byte *iv) {php_printf("ThrowIfInvalidIV\n"); CryptoPP::AES::Encryption::ThrowIfInvalidIV(iv);};
        size_t ThrowIfInvalidIVLength(int size) {php_printf("ThrowIfInvalidIVLength\n"); return CryptoPP::AES::Encryption::ThrowIfInvalidIVLength(size);};
        const byte * GetIVAndThrowIfInvalid(const CryptoPP::NameValuePairs &params, size_t &size) {php_printf("GetIVAndThrowIfInvalid\n"); return CryptoPP::AES::Encryption::GetIVAndThrowIfInvalid(params, size);};
        inline void AssertValidKeyLength(size_t length) const {php_printf("AssertValidKeyLength\n"); CryptoPP::AES::Encryption::AssertValidKeyLength(length);};

        unsigned int m_blockSize;
        zval *m_blockCipherObject;
        zval *m_funcnameProcessData;
        zval *m_funcnameIsValidKeyLength;
        zval *m_funcnameSetKey;
    };
    /* }}} */

    /* {{{ encryption class */
    class Encryption : public Base
    {
    public:
        Encryption(zval *blockCipherObject) : Base(blockCipherObject, "encryptData"){};
        bool IsForwardTransformation() const {return true;};
    };
    /* }}} */

    /* {{{ decryption class */
    class Decryption : public Base
    {
    public:
        Decryption(zval *blockCipherObject) : Base(blockCipherObject, "decryptData"){};
        bool IsForwardTransformation() const {return false;};
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
