/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_MAC_CMAC_H
#define PHP_MAC_CMAC_H

#include "src/php_cryptopp.h"
#include <cmac.h>

void init_class_MacCmac(TSRMLS_D);
PHP_METHOD(Cryptopp_MacCmac, __construct);

/* {{{ fork of CryptoPP::CMAC that take a cipher as parameter instead of a template parameter */
class Cmac : public CryptoPP::CMAC_Base
{
public:
    Cmac(CryptoPP::BlockCipher *cipher, bool freeCipherObject, zval *zThis);
    ~Cmac();

    bool IsValidKeyLength(size_t n) const;
    void UncheckedSetKey(const byte *userKey, unsigned int keylength, const CryptoPP::NameValuePairs &params);

    size_t MinKeyLength() const {return m_cipher->MinKeyLength();}
    size_t MaxKeyLength() const {return m_cipher->MaxKeyLength();}
    size_t DefaultKeyLength() const {return m_cipher->DefaultKeyLength();}
    size_t GetValidKeyLength(size_t n) const {return m_cipher->GetValidKeyLength(n);}
    SimpleKeyingInterface::IV_Requirement IVRequirement() const {return m_cipher->IVRequirement();}
    unsigned int IVSize() const {return m_cipher->IVSize();}

    static std::string StaticAlgorithmName() {return std::string("CMAC");}
    std::string AlgorithmName() const {return std::string("CMAC(") + m_cipher->AlgorithmName() + ")";}

private:
    CryptoPP::BlockCipher & AccessCipher() {return *m_cipher;}

    CryptoPP::BlockCipher *m_cipher;
    bool m_freeCipherObject;
    zval *m_zThis;
};
/* }}} */

#endif /* PHP_MAC_CMAC_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
