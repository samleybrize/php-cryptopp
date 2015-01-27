/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_AUTHENTICATED_SYMMETRIC_CIPHER_EAX_H
#define PHP_AUTHENTICATED_SYMMETRIC_CIPHER_EAX_H

#include "src/php_cryptopp.h"
#include <eax.h>

void init_class_AuthenticatedSymmetricCipherEax(TSRMLS_D);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherEax, __construct);

/* {{{ fork of CryptoPP::CMAC that take a cipher as parameter instead of a template parameter */
class CMAC : public CryptoPP::CMAC_Base
{
public:
    CMAC(CryptoPP::BlockCipher *cipher) : m_cipher(cipher) {assert(cipher->IsForwardTransformation());}
    size_t DefaultKeyLength() const {return m_cipher->DefaultKeyLength();}
    size_t MinKeyLength() const {return m_cipher->MinKeyLength();}
    size_t MaxKeyLength() const {return m_cipher->MaxKeyLength();}
    size_t GetValidKeyLength(size_t n) const {return m_cipher->GetValidKeyLength(n);}
    bool IsValidKeyLength(size_t n) const {return m_cipher->IsValidKeyLength(n);}
    IV_Requirement IVRequirement() const {return NOT_RESYNCHRONIZABLE;}

private:
    CryptoPP::BlockCipher & AccessCipher() {return *m_cipher;}
    CryptoPP::BlockCipher *m_cipher;
};
/* }}} */

/* {{{ fork of CryptoPP::EAX that take a cipher as parameter instead of a template parameter */
class EAX
{
public:
    /* {{{ base class */
    class Base : public CryptoPP::EAX_Base
    {
    public:
        ~Base();
        static std::string StaticAlgorithmName() {return std::string("EAX");}

    protected:
        Base(CryptoPP::BlockCipher *cipher, bool cipherMustBeDestructed);

    private:
        CryptoPP::BlockCipher & AccessBlockCipher() {return *m_cipher;}
        CryptoPP::CMAC_Base & AccessMAC() {return m_cmac;}

        bool m_cipherMustBeDestructed;
        CryptoPP::BlockCipher *m_cipher;
        CMAC m_cmac;
    };
    /* }}} */

    /* {{{ encryption class */
    class Encryption : public Base
    {
    public:
        Encryption(CryptoPP::BlockCipher *cipher, bool cipherMustBeDestructed) : Base(cipher, cipherMustBeDestructed){};
        bool IsForwardTransformation() const {return true;}
    };
    /* }}} */

    /* {{{ decryption class */
    class Decryption : public Base
    {
    public:
        Decryption(CryptoPP::BlockCipher *cipher, bool cipherMustBeDestructed) : Base(cipher, cipherMustBeDestructed){};
        bool IsForwardTransformation() const {return false;}
    };
    /* }}} */
};
/* }}} */

#endif /* PHP_AUTHENTICATED_SYMMETRIC_CIPHER_EAX_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
