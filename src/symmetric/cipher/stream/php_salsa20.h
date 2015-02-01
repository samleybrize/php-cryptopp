/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_STREAM_CIPHER_SALSA20_H
#define PHP_STREAM_CIPHER_SALSA20_H

#include "src/php_cryptopp.h"
#include <salsa.h>

void init_class_StreamCipherSalsa20(TSRMLS_D);
PHP_METHOD(Cryptopp_StreamCipherSalsa20, __construct);
PHP_METHOD(Cryptopp_StreamCipherSalsa20, setRounds);

/* {{{ fork of CryptoPP::Salsa20 that allow to set the number of rounds */
class Salsa20
{
public:
    /* {{{ encryption class */
    class Encryption : public CryptoPP::Salsa20::Encryption
    {
    public:
        Encryption() : CryptoPP::Salsa20::Encryption(){};
        void SetRounds(int rounds);
        void CipherSetKey(const CryptoPP::NameValuePairs &params, const byte *key, size_t length);
    };
    /* }}} */

    /* {{{ decryption class */
    class Decryption : public CryptoPP::Salsa20::Decryption
    {
    public:
        Decryption() : CryptoPP::Salsa20::Decryption(){};
        void SetRounds(int rounds);
        void CipherSetKey(const CryptoPP::NameValuePairs &params, const byte *key, size_t length);
    };
    /* }}} */
};
/* }}} */

#endif /* PHP_STREAM_CIPHER_SALSA20_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
