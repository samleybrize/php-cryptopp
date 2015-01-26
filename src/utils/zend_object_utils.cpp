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
#include "src/symmetric/cipher/symmetric_transformation_user_interface.h"
#include "zend_object_utils.h"
#include <zend_exceptions.h>

/* {{{ verify that a key size is valid for a given algorithm */
bool isCryptoppSymmetricKeyValid(zval *object, CryptoPP::SimpleKeyingInterface *keying, int keySize TSRMLS_DC, bool throwIfFalse) {
    if (!keying->IsValidKeyLength(keySize)) {
        zend_class_entry *ce;
        ce = zend_get_class_entry(object TSRMLS_CC);

        if (!throwIfFalse) {
            return false;
        } else if (0 == keySize) {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : a key is required", ce->name, keySize);
        } else {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : %d is not a valid key length", ce->name, keySize);
        }

        return false;
    }

    return true;
}
/* }}} */

/* {{{ verify that an IV size is valid for a given algorithm */
bool isCryptoppSymmetricIvValid(zval *object, CryptoPP::SimpleKeyingInterface *keying, int ivSize TSRMLS_DC, bool throwIfFalse) {
    bool isValid = false;

    if (0 != dynamic_cast<SymmetricTransformationUserInterface*>(keying)) {
        isValid = dynamic_cast<SymmetricTransformationUserInterface*>(keying)->IsValidIvLength(ivSize);
    } else if(!keying->IsResynchronizable()) {
        isValid = (0 == ivSize);
    } else {
        isValid = ivSize >= keying->MinIVLength() && ivSize <= keying->MaxIVLength();
    }

    if (!isValid) {
        zend_class_entry *ce;
        ce = zend_get_class_entry(object TSRMLS_CC);

        if (!throwIfFalse) {
            return false;
        } else if (!keying->IsResynchronizable()) {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : no initialization vector needed", ce->name, ivSize);
        } else if (0 == ivSize) {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : an initialization vector is required", ce->name, ivSize);
        } else {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : %d is not a valid initialization vector length", ce->name, ivSize);
        }

        return false;
    }

    return true;
}
/* }}} */

/* {{{ sets the key and the iv (if applicable) of the native cipher objects of a cipher php object */
bool setSymmetricCipherKeyIv(
    zval *object,
    CryptoPP::SimpleKeyingInterface *encryptor,
    CryptoPP::SimpleKeyingInterface *decryptor,
    zval *zKey,
    zval *zIv
    TSRMLS_DC
) {
    CryptoPP::SimpleKeyingInterface *cipher = NULL != encryptor ? encryptor : decryptor;
    int keySize                             = IS_STRING == Z_TYPE_P(zKey) ? Z_STRLEN_P(zKey) : 0;
    int ivSize                              = IS_STRING == Z_TYPE_P(zIv) ? Z_STRLEN_P(zIv) : 0;
    bool ivSetted                           = false;

    // set the key and the iv (if applicable) of native mode objects
    if (keySize > 0 && !cipher->IsResynchronizable()) {
        // an iv is not required
        ivSetted = true;
    } else if (keySize > 0 && ivSize > 0 && cipher->IsResynchronizable()) {
        // set key and iv
        byte *key   = reinterpret_cast<byte*>(Z_STRVAL_P(zKey));
        byte *iv    = reinterpret_cast<byte*>(Z_STRVAL_P(zIv));

        if (NULL != encryptor) {
            encryptor->SetKeyWithIV(key, keySize, iv, ivSize);
        }

        if (NULL != decryptor) {
            decryptor->SetKeyWithIV(key, keySize, iv, ivSize);
        }

        // indicates that the iv is setted
        ivSetted = true;
    }

    return ivSetted;
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
