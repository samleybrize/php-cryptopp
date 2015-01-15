#ifndef PHP_MAC_HMAC_H
#define PHP_MAC_HMAC_H

#include "src/php_cryptopp.h"
#include <hmac.h>
#include <seckey.h>

/* {{{ fork of CryptoPP::HMAC that take a hash as parameter instead of a template parameter */
class Hmac : public CryptoPP::MessageAuthenticationCodeImpl<CryptoPP::HMAC_Base, Hmac>
{
public:
    Hmac(CryptoPP::HashTransformation *hash, bool freeHashObject);
    ~Hmac();

    static std::string StaticAlgorithmName() {return std::string("HMAC");}
    std::string AlgorithmName() const {return std::string("HMAC(") + m_hash->AlgorithmName() + ")";}

private:
    CryptoPP::HashTransformation & AccessHash() {return *m_hash;}

    CryptoPP::HashTransformation *m_hash;
    bool m_freeHashObject;
};
/* }}} */

void init_class_MacHmac(TSRMLS_D);
PHP_METHOD(Cryptopp_MacHmac, __construct);

#endif /* PHP_MAC_HMAC_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
