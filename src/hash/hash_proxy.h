/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_HASH_PROXY_H
#define PHP_HASH_PROXY_H

#include "src/php_cryptopp.h"

/* {{{ TODO */
class HashProxy : public CryptoPP::HashTransformation
{
public:
    HashProxy(zval *hashObject TSRMLS_DC);
    ~HashProxy();
    unsigned int DigestSize() const;
    unsigned int BlockSize() const;
    void Update(const byte *input, size_t length);
    virtual void Final(byte *digest);
    void TruncatedFinal(byte *digest, size_t digestSize);
    void Restart();
    void CalculateDigest(byte *digest, const byte *input, size_t length);
    unsigned int OptimalBlockSize() const;

    static const char * CRYPTOPP_API StaticAlgorithmName() {return "User";}

    // unused
    unsigned int OptimalDataAlignment() const {return 1;}
    byte * CreateUpdateSpace(size_t &size) {return NULL;}
    bool Verify(const byte *digest) {return true;}
    bool VerifyDigest(const byte *digest, const byte *input, size_t length) {return true;}
    void CalculateTruncatedDigest(byte *digest, size_t digestSize, const byte *input, size_t length) {}
    bool TruncatedVerify(const byte *digest, size_t digestLength) {return true;}
    bool VerifyTruncatedDigest(const byte *digest, size_t digestLength, const byte *input, size_t length) {return true;}

protected:
    zval *m_hashObject;
    unsigned int m_blockSize;
    unsigned int m_digestSize;
    zval *m_funcnameCalculateDigest;
    zval *m_funcnameUpdate;
    zval *m_funcnameFinal;
    zval *m_funcnameRestart;
    M_TSRMLS_D;
};
/* }}} */

#endif /* PHP_HASH_PROXY_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
