#ifndef PHP_AUTHENTICATED_SYMMETRIC_CIPHER_INTERFACE_H
#define PHP_AUTHENTICATED_SYMMETRIC_CIPHER_INTERFACE_H

#include "../../../php_cryptopp.h"

/* {{{ php authenticated cipher classes methods arg info */
ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherInterface_getDigestSize, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherInterface_addEncryptionAdditionalData, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherInterface_addDecryptionAdditionalData, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherInterface_finalizeEncryption, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipherInterface_finalizeDecryption, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* inits the AuthenticatedSymmetricCipherInterface php interface */
extern zend_class_entry *cryptopp_ce_AuthenticatedSymmetricCipherInterface;
void init_interface_AuthenticatedSymmetricCipherInterface(TSRMLS_D);

#endif /* PHP_AUTHENTICATED_SYMMETRIC_CIPHER_INTERFACE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
