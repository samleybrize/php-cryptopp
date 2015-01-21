/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_AUTHENTICATED_SYMMETRIC_CIPHER_CCM_H
#define PHP_AUTHENTICATED_SYMMETRIC_CIPHER_CCM_H

#include "src/php_cryptopp.h"
#include <ccm.h>

/* {{{ fork of CryptoPP::CCM that take a cipher as parameter instead of a template parameter */
class CCM : public CryptoPP::CCM_Base
{
public:
    /* {{{ base class */
    class Base : public CryptoPP::CCM_Base
    {
    public:
        ~Base();
        void SetDigestSize(int digestSize);
        static std::string StaticAlgorithmName() {return std::string("CCM");}

    protected:
        Base(CryptoPP::BlockCipher *cipher, bool cipherMustBeDestructed TSRMLS_DC);
        int DefaultDigestSize() const {return DigestSize();}

    private:
        CryptoPP::BlockCipher & AccessBlockCipher() {return *m_cipher;}

        bool m_cipherMustBeDestructed;
        CryptoPP::BlockCipher *m_cipher;
        M_TSRMLS_D;
    };
    /* }}} */

    /* {{{ encryption class */
    class Encryption : public Base
    {
    public:
        Encryption(CryptoPP::BlockCipher *cipher, bool cipherMustBeDestructed TSRMLS_DC) : Base(cipher, cipherMustBeDestructed TSRMLS_CC){};
        bool IsForwardTransformation() const {return true;}
    };
    /* }}} */

    /* {{{ decryption class */
    class Decryption : public Base
    {
    public:
        Decryption(CryptoPP::BlockCipher *cipher, bool cipherMustBeDestructed TSRMLS_DC) : Base(cipher, cipherMustBeDestructed TSRMLS_CC){};
        bool IsForwardTransformation() const {return false;}
    };
    /* }}} */
};
/* }}} */

void init_class_AuthenticatedSymmetricCipherCcm(TSRMLS_D);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherCcm, __construct);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherCcm, setDigestSize);
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherCcm, specifyDataSize);

#endif /* PHP_AUTHENTICATED_SYMMETRIC_CIPHER_CCM_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
