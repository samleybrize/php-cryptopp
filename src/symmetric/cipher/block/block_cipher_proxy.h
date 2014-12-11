#ifndef PHP_BLOCK_CIPHER_PROXY_H
#define PHP_BLOCK_CIPHER_PROXY_H

#include "../../../php_cryptopp.h"
#include "php_block_cipher_interface.h"
#include <string>

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
        void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock);
        std::string AlgorithmName() const {return m_name;}

        // unused
        size_t GetValidKeyLength(size_t n) const {return 1;}
        unsigned int OptimalDataAlignment() const {return 1;}
        bool IsPermutation() const {return true;}
        unsigned int OptimalNumberOfParallelBlocks() const {return 1;}
        size_t MinKeyLength() const {return 1;}
        size_t MaxKeyLength() const {return 1;}
        size_t DefaultKeyLength() const {return 1;}
        IV_Requirement IVRequirement() const {return NOT_RESYNCHRONIZABLE;}
        unsigned int IVSize() const {return 0;}
        unsigned int MinIVLength() const {return 0;}
        unsigned int MaxIVLength() const {return 0;}
        void Resynchronize(const byte *iv, int ivLength=-1) {}
        void GetNextIV(CryptoPP::RandomNumberGenerator &rng, byte *IV) {}

    protected:
        Base(zval *blockCipherObject, const char* processDataFuncname, const char *processBlockFuncname);

        // unused
        void UncheckedSetKey(const byte *key, unsigned int length, const CryptoPP::NameValuePairs &params) {};

        std::string m_name;
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
