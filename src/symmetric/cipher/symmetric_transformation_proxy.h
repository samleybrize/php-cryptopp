#ifndef PHP_SYMMETRIC_TRANSFORMATION_PROXY_H
#define PHP_SYMMETRIC_TRANSFORMATION_PROXY_H

#include "../../php_cryptopp.h"
#include "symmetric_transformation_user_interface.h"

class SymmetricTransformationProxy
{
    /* {{{ base class */
    class Base : public CryptoPP::SymmetricCipher, public SymmetricTransformationUserInterface
    {
    public:
        ~Base();
        unsigned int MandatoryBlockSize() const;
        unsigned int OptimalBlockSize() const;
        void ProcessData(byte *outString, const byte *inString, size_t length);
        bool IsValidKeyLength(size_t n) const;
        bool IsValidKeyLength(size_t n);
        bool IsValidIvLength(size_t n);
        void SetKeyWithIV(const byte *key, size_t length, const byte *iv, size_t ivLength);
        void SetKey(const byte *key, size_t length, const CryptoPP::NameValuePairs &params = CryptoPP::g_nullNameValuePairs);

        // unused methods
        IV_Requirement IVRequirement() const {return RANDOM_IV;}
        bool IsRandomAccess() const {return false;}
        bool IsSelfInverting() const {return false;}

        size_t MinKeyLength() const {return 1;}
        size_t MaxKeyLength() const {return 1;}
        size_t DefaultKeyLength() const {return 1;}
        size_t GetValidKeyLength(size_t n) const {return 1;}
        void UncheckedSetKey(const byte *key, unsigned int length, const CryptoPP::NameValuePairs &params) {}

    protected:
        Base(zval *symmetricTransformationObject, const char* processDataFuncname);

        unsigned int m_blockSize;
        zval *m_symmetricTransformationObject;
        zval *m_processDataFuncname;
        zval *m_funcnameIsValidKeyLength;
        zval *m_funcnameIsValidIvLength;
        zval *m_funcnameSetKey;
        zval *m_funcnameSetIv;
    };
    /* }}} */

public:
    /* {{{ encryption class */
    class Encryption : public Base
    {
    public:
        Encryption(zval *symmetricTransformationObject) : Base(symmetricTransformationObject, "encrypt"){};
        bool IsForwardTransformation() const {return true;};
    };
    /* }}} */

    /* {{{ decryption class */
    class Decryption : public Base
    {
    public:
        Decryption(zval *symmetricTransformationObject) : Base(symmetricTransformationObject, "decrypt"){};
        bool IsForwardTransformation() const {return false;};
    };
    /* }}} */
};

#endif /* PHP_SYMMETRIC_TRANSFORMATION_PROXY_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
