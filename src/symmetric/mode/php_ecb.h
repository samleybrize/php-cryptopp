#ifndef PHP_SYMMETRIC_MODE_ECB_H
#define PHP_SYMMETRIC_MODE_ECB_H

#include "src/php_cryptopp.h"
#include <modes.h>

void init_class_SymmetricModeEcb(TSRMLS_D);
PHP_METHOD(Cryptopp_SymmetricModeEcb, __construct);

/* {{{ fork of CryptoPP::ECB_Mode_ExternalCipher to support block cipher destruction */
class Ecb
{
public:
    /* {{{ base class */
    class Base
    {
    public:
        ~Base();

    protected:
        Base(CryptoPP::BlockCipher *cipher, bool cipherMustBeDestructed)
            : m_cipher(cipher)
            , m_cipherMustBeDestructed(cipherMustBeDestructed) {};

        CryptoPP::BlockCipher * m_cipher;
        bool m_cipherMustBeDestructed;
    };
    /* }}} */

    /* {{{ encryption class */
    class Encryption : public Base, public CryptoPP::ECB_Mode_ExternalCipher::Encryption
    {
    public:
        Encryption(CryptoPP::BlockCipher *cipher, bool cipherMustBeDestructed)
            : Base(cipher, cipherMustBeDestructed)
            , CryptoPP::ECB_Mode_ExternalCipher::Encryption(*cipher) {};
    };
    /* }}} */

    /* {{{ decryption class */
    class Decryption : public Base, public CryptoPP::ECB_Mode_ExternalCipher::Decryption
    {
    public:
        Decryption(CryptoPP::BlockCipher *cipher, bool cipherMustBeDestructed)
            : Base(cipher, cipherMustBeDestructed)
            , CryptoPP::ECB_Mode_ExternalCipher::Decryption(*cipher) {};
    };
    /* }}} */
};
/* }}} */

#endif /* PHP_SYMMETRIC_MODE_ECB_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
