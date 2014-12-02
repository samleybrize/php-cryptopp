#ifndef PHP_RBG_H
#define PHP_RBG_H

#include "../php_cryptopp.h"

void init_class_RandomByteGenerator(TSRMLS_D);
PHP_METHOD(Cryptopp_RandomByteGenerator, __construct);
PHP_METHOD(Cryptopp_RandomByteGenerator, __sleep);
PHP_METHOD(Cryptopp_RandomByteGenerator, __wakeup);
PHP_METHOD(Cryptopp_RandomByteGenerator, generate);

/* {{{ object creation related stuff */
struct RandomByteGeneratorContainer {
    zend_object std;
    CryptoPP::RandomNumberGenerator *rbg;
};
/* }}} */

#endif /* PHP_RBG_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
