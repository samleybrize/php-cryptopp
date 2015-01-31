/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_SYMMETRIC_MODE_CTR_H
#define PHP_SYMMETRIC_MODE_CTR_H

#include "src/php_cryptopp.h"
#include <modes.h>

void init_class_SymmetricModeCtr(TSRMLS_D);
PHP_METHOD(Cryptopp_SymmetricModeCtr, __construct);

/* {{{ fork of CryptoPP::CTR_Mode_ExternalCipher to support block cipher destruction */
class Ctr
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
    class Encryption : public Base, public CryptoPP::CTR_Mode_ExternalCipher::Encryption
    {
    public:
        Encryption(CryptoPP::BlockCipher *cipher, bool cipherMustBeDestructed, byte *iv, int ivSize)
            : Base(cipher, cipherMustBeDestructed)
            , CryptoPP::CTR_Mode_ExternalCipher::Encryption(*cipher, iv, ivSize) {};
    };
    /* }}} */

    /* {{{ decryption class */
    class Decryption : public Base, public CryptoPP::CTR_Mode_ExternalCipher::Decryption
    {
    public:
        Decryption(CryptoPP::BlockCipher *cipher, bool cipherMustBeDestructed, byte *iv, int ivSize)
            : Base(cipher, cipherMustBeDestructed)
            , CryptoPP::CTR_Mode_ExternalCipher::Decryption(*cipher, iv, ivSize) {};
    };
    /* }}} */
};
/* }}} */

#endif /* PHP_SYMMETRIC_MODE_CTR_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
