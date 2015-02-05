/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_STREAM_CIPHER_XSALSA20_H
#define PHP_STREAM_CIPHER_XSALSA20_H

#include "src/php_cryptopp.h"
#include <salsa.h>

void init_class_StreamCipherXSalsa20(TSRMLS_D);
PHP_METHOD(Cryptopp_StreamCipherXSalsa20, __construct);
PHP_METHOD(Cryptopp_StreamCipherXSalsa20, setRounds);

/* {{{ fork of CryptoPP::XSalsa20 that allow to set the number of rounds */
class XSalsa20
{
public:
    /* {{{ base class */
    class Base
    {
    protected:
        Base(zval *object TSRMLS_DC);

        zval *m_object;
        M_TSRMLS_D;
    };
    /* }}} */

    /* {{{ encryption class */
    class Encryption : public Base, public CryptoPP::XSalsa20::Encryption
    {
    public:
        Encryption(zval *object TSRMLS_DC);

        void SetRounds(int rounds);
        void CipherSetKey(const CryptoPP::NameValuePairs &params, const byte *key, size_t length);
    };
    /* }}} */

    /* {{{ decryption class */
    class Decryption : public Base, public CryptoPP::XSalsa20::Decryption
    {
    public:
        Decryption(zval *object TSRMLS_DC);

        void SetRounds(int rounds);
        void CipherSetKey(const CryptoPP::NameValuePairs &params, const byte *key, size_t length);
    };
    /* }}} */
};
/* }}} */

#endif /* PHP_STREAM_CIPHER_XSALSA20_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
