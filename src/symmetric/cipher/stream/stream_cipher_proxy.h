#ifndef PHP_STREAM_CIPHER_PROXY_H
#define PHP_STREAM_CIPHER_PROXY_H

#include "../../../php_cryptopp.h"

class StreamCipherProxy
{
public:
    /* {{{ base class */
    class Base : public CryptoPP::SymmetricCipher
    {
    public:
        Base(zval *streamCipherObject);

        unsigned int MandatoryBlockSize() const;
        unsigned int OptimalBlockSize() const;

        // TODO unused methods
        IV_Requirement IVRequirement() const {php_printf("IVRequirement\n"); return UNPREDICTABLE_RANDOM_IV;}
        bool IsRandomAccess() const {php_printf("IsRandomAccess\n"); return false;}
        bool IsSelfInverting() const {php_printf("IsSelfInverting\n"); return false;}

        size_t MinKeyLength() const {php_printf("MinKeyLength\n"); return 1;}
        size_t MaxKeyLength() const {php_printf("MaxKeyLength\n"); return 1;}
        size_t DefaultKeyLength() const {php_printf("DefaultKeyLength\n"); return 1;}
        size_t GetValidKeyLength(size_t n) const {php_printf("GetValidKeyLength\n"); return 1;}
        void UncheckedSetKey(const byte *key, unsigned int length, const CryptoPP::NameValuePairs &params) {php_printf("UncheckedSetKey\n");}
    };
    /* }}} */

    /* {{{ encryption class */
    class Encryption : public Base
    {
    public:
        bool IsForwardTransformation() const {return false;};
        void ProcessData(byte *outString, const byte *inString, size_t length);
    };
    /* }}} */

    /* {{{ decryption class */
    class Decryption : public Base
    {
    public:
        bool IsForwardTransformation() const {return false;};
        void ProcessData(byte *outString, const byte *inString, size_t length);
    };
    /* }}} */
};

#endif /* PHP_STREAM_CIPHER_PROXY_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
