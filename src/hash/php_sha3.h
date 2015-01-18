/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_HASH_SHA3_H
#define PHP_HASH_SHA3_H

#include "src/php_cryptopp.h"
#include <sha3.h>

void init_classes_HashSha3(TSRMLS_D);
PHP_METHOD(Cryptopp_HashSha3_224, __construct);
PHP_METHOD(Cryptopp_HashSha3_256, __construct);
PHP_METHOD(Cryptopp_HashSha3_384, __construct);
PHP_METHOD(Cryptopp_HashSha3_512, __construct);

/* {{{ adds blocksize specification to SHA3_* classes. see http://csrc.nist.gov/groups/ST/hash/sha-3/Aug2014/documents/perlner_kmac.pdf */
class SHA3_224 : public CryptoPP::SHA3_224
{
public:
    unsigned int BlockSize() const {return 72;}
};

class SHA3_256 : public CryptoPP::SHA3_256
{
public:
    unsigned int BlockSize() const {return 104;}
};

class SHA3_384 : public CryptoPP::SHA3_384
{
public:
    unsigned int BlockSize() const {return 136;}
};

class SHA3_512 : public CryptoPP::SHA3_512
{
public:
    unsigned int BlockSize() const {return 144;}
};
/* }}} */

#endif /* PHP_HASH_SHA3_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
