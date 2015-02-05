/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "src/php_cryptopp.h"
#include "src/exception/php_exception.h"
#include "src/symmetric/cipher/block/php_block_cipher_interface.h"
#include "src/symmetric/cipher/block/php_block_cipher_abstract.h"
#include "src/symmetric/cipher/block/block_cipher_proxy.h"
#include "src/utils/zval_utils.h"
#include "php_mac_abstract.h"
#include "php_cmac.h"
#include <cmac.h>
#include <string>
#include <zend_exceptions.h>

/* {{{ fork of CryptoPP::CMAC that take a cipher as parameter instead of a template parameter */
Cmac::Cmac(CryptoPP::BlockCipher *cipher, bool freeCipherObject)
{
    m_cipher            = cipher;
    m_freeCipherObject  = freeCipherObject;
}

Cmac::~Cmac()
{
    if (m_freeCipherObject) {
        delete m_cipher;
    }
}

bool Cmac::IsValidKeyLength(size_t n) const
{
    return m_cipher->IsValidKeyLength(n);
}
/* }}} */

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_MacCmac_construct, 0)
    ZEND_ARG_OBJ_INFO(0, cipher, Cryptopp\\BlockCipherInterface, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_MacCmac;

static zend_function_entry cryptopp_methods_MacCmac[] = {
    PHP_ME(Cryptopp_MacCmac, __construct, arginfo_MacCmac_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

void init_class_MacCmac(TSRMLS_D) {
    init_class_MacAbstractChild("cmac", "MacCmac", &cryptopp_ce_MacCmac, cryptopp_methods_MacCmac TSRMLS_CC);
    zend_declare_property_null(cryptopp_ce_MacCmac, "cipher", 6,  ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ proto MacCmac::__construct(Cryptopp\BlockCipherInterface cipher) */
PHP_METHOD(Cryptopp_MacCmac, __construct) {
    zval *cipherObject;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &cipherObject, cryptopp_ce_BlockCipherInterface)) {
        return;
    }

    CryptoPP::MessageAuthenticationCode *mac;
    CryptoPP::BlockCipher *cipher;

    if (instanceof_function(Z_OBJCE_P(cipherObject), cryptopp_ce_BlockCipherAbstract TSRMLS_CC)) {
        // retrieve native cipher object
        cipher  = getCryptoppBlockCipherEncryptorPtr(cipherObject TSRMLS_CC);
        mac     = new Cmac(cipher, false);
    } else {
        // create a proxy to the user php object
        try {
            cipher  = new BlockCipherProxy::Encryption(cipherObject TSRMLS_CC);
            mac     = new Cmac(cipher, true);
        } catch (bool e) {
            return;
        }
    }

    setCryptoppMacNativePtr(getThis(), mac  TSRMLS_CC);

    // compute algo name
    zval *funcName      = makeZval("getName");
    zval *cipherName    = call_user_method(cipherObject, funcName TSRMLS_CC);

    std::string name("cmac(");
    name.append(Z_STRVAL_P(cipherName), Z_STRLEN_P(cipherName));
    name.append(")");
    zend_update_property_stringl(cryptopp_ce_MacAbstract, getThis(), "name", 4, name.c_str(), name.length() TSRMLS_CC);

    zval_ptr_dtor(&cipherName);
    zval_ptr_dtor(&funcName);

    // hold the cipher object. if not, it can be deleted.
    zend_update_property(cryptopp_ce_MacCmac, getThis(), "cipher", 6, cipherObject TSRMLS_CC);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
