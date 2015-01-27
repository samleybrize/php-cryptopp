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
#include "src/utils/zend_object_utils.h"
#include "src/hash/php_hash_transformation_interface.h"
#include "php_mac.h"
#include "php_mac_interface.h"
#include "php_mac_abstract.h"
#include <zend_exceptions.h>
#include <string>

/* {{{ MacAbstract_free_storage
   custom object free handler */
void MacAbstract_free_storage(void *object TSRMLS_DC) {
    MacAbstractContainer *obj = static_cast<MacAbstractContainer *>(object);
    delete obj->mac;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}
/* }}} */

/* {{{ PHP abstract class declaration */
zend_object_handlers MacAbstract_object_handlers;
zend_class_entry *cryptopp_ce_MacAbstract;

static zend_function_entry cryptopp_methods_MacAbstract[] = {
    PHP_ME(Cryptopp_MacAbstract, __sleep, arginfo_MacAbstract___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_MacAbstract, __wakeup, arginfo_MacAbstract___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_MacAbstract, getName, arginfo_HashTransformationInterface_getName, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_MacAbstract, getDigestSize, arginfo_HashTransformationInterface_getDigestSize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_MacAbstract, getBlockSize, arginfo_HashTransformationInterface_getBlockSize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_MacAbstract, isValidKeyLength, arginfo_MacInterface_isValidKeyLength, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_MacAbstract, setKey, arginfo_MacInterface_setKey, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_MacAbstract, getKey, arginfo_MacInterface_getKey, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_MacAbstract, calculateDigest, arginfo_HashTransformationInterface_calculateDigest, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_MacAbstract, update, arginfo_HashTransformationInterface_update, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_MacAbstract, finalize, arginfo_HashTransformationInterface_finalize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_MacAbstract, restart, arginfo_HashTransformationInterface_restart, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_MacAbstract(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "MacAbstract", cryptopp_methods_MacAbstract);
    cryptopp_ce_MacAbstract                 = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_MacAbstract->ce_flags      |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

    cryptopp_ce_MacAbstract->create_object  = zend_custom_create_handler<MacAbstractContainer, MacAbstract_free_storage, &MacAbstract_object_handlers>;
    memcpy(&MacAbstract_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    MacAbstract_object_handlers.clone_obj   = NULL;

    zend_class_implements(cryptopp_ce_MacAbstract TSRMLS_CC, 1, cryptopp_ce_MacInterface);

    zend_declare_property_string(cryptopp_ce_MacAbstract, "name", 4, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_string(cryptopp_ce_MacAbstract, "key", 3, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ init_class_MacAbstractChild
   inits a child class */
void init_class_MacAbstractChild(const char *algoName, const char* className, zend_class_entry **classEntryPtr, zend_function_entry *classMethods TSRMLS_DC) {
    std::string namespacedClassName("Cryptopp\\");
    namespacedClassName.append(className);

    zend_class_entry ce;
    INIT_CLASS_ENTRY_EX(ce, namespacedClassName.c_str(), namespacedClassName.length(), classMethods);
    *classEntryPtr = zend_register_internal_class_ex(&ce, cryptopp_ce_MacAbstract, NULL TSRMLS_CC);

    macAlgoList.addAlgo(algoName, namespacedClassName.c_str());
}
/* }}} */

/* {{{ isCryptoppMacKeyValid
   verify that a key size is valid for a MacAbstract instance */
bool isCryptoppMacKeyValid(zval *object, CryptoPP::MessageAuthenticationCode *mac TSRMLS_DC) {
    zval *key   = zend_read_property(cryptopp_ce_MacAbstract, object, "key", 3, 1 TSRMLS_CC);
    int keySize = Z_STRLEN_P(key);

    return isCryptoppSymmetricKeyValid(object, mac, keySize TSRMLS_CC);
}
/* }}} */

/* {{{ getCryptoppMacNativePtr
   get the pointer to the native mac object of a php mac class */
CryptoPP::MessageAuthenticationCode *getCryptoppMacNativePtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::MessageAuthenticationCode *mac = static_cast<MacAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->mac;

    if (NULL == mac) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\MacAbstract cannot be extended by user classes");
    }

    return mac;
}
/* }}} */

/* {{{ setCryptoppMacNativePtr
   set the pointer to the native mac object of a php mac class */
void setCryptoppMacNativePtr(zval *this_ptr, CryptoPP::MessageAuthenticationCode *nativePtr TSRMLS_DC) {
    static_cast<MacAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->mac = nativePtr;
}
/* }}} */

/* {{{ proto void MacAbstract::__sleep(void)
   Prevents serialization of a MacAbstract instance */
PHP_METHOD(Cryptopp_MacAbstract, __sleep) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\MacAbstract instances");
}
/* }}} */

/* {{{ proto void MacAbstract::__wakeup(void)
   Prevents use of a MacAbstract instance that has been unserialized */
PHP_METHOD(Cryptopp_MacAbstract, __wakeup) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\MacAbstract instances");
}
/* }}} */

/* {{{ proto int MacAbstract::getDigestSize(void)
   Returns the digest size */
PHP_METHOD(Cryptopp_MacAbstract, getDigestSize) {
    CryptoPP::MessageAuthenticationCode *mac;
    CRYPTOPP_MAC_ABSTRACT_GET_NATIVE_PTR(mac)

    unsigned int digestSize = mac->DigestSize();
    RETURN_LONG(digestSize);
}
/* }}} */

/* {{{ proto int MacAbstract::getBlockSize(void)
   Returns the block size */
PHP_METHOD(Cryptopp_MacAbstract, getBlockSize) {
    CryptoPP::MessageAuthenticationCode *mac;
    CRYPTOPP_MAC_ABSTRACT_GET_NATIVE_PTR(mac)

    unsigned int blockSize = mac->BlockSize();
    RETURN_LONG(blockSize);
}
/* }}} */

/* {{{ proto bool MacAbstract::isValidKeyLength(int keyLength)
   Indicates if a key length is valid */
PHP_METHOD(Cryptopp_MacAbstract, isValidKeyLength) {
    long keySize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &keySize)) {
        return;
    }

    CryptoPP::MessageAuthenticationCode *mac;
    CRYPTOPP_MAC_ABSTRACT_GET_NATIVE_PTR(mac)

    if (mac->IsValidKeyLength(keySize)) {
        RETURN_TRUE
    } else {
        RETURN_FALSE
    }
}
/* }}} */

/* {{{ proto string MacAbstract::getName(void)
   Return algorithm name */
PHP_METHOD(Cryptopp_MacAbstract, getName) {
    CryptoPP::MessageAuthenticationCode *mac;
    CRYPTOPP_MAC_ABSTRACT_GET_NATIVE_PTR(mac)
    zval *name = zend_read_property(cryptopp_ce_MacAbstract, getThis(), "name", 4, 0 TSRMLS_CC);
    RETURN_ZVAL(name, 1, 0);
}
/* }}} */

/* {{{ proto void MacAbstract::setKey(string key)
   Sets the key */
PHP_METHOD(Cryptopp_MacAbstract, setKey) {
    char *key   = NULL;
    int keySize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &keySize)) {
        return;
    }

    CryptoPP::MessageAuthenticationCode *mac;
    CRYPTOPP_MAC_ABSTRACT_GET_NATIVE_PTR(mac)

    if (!isCryptoppSymmetricKeyValid(getThis(), mac, keySize TSRMLS_CC)) {
        RETURN_FALSE;
    }

    // set the key on both the php object and the native cryptopp object
    mac->SetKey(reinterpret_cast<byte*>(key), keySize);
    mac->Restart();
    zend_update_property_stringl(cryptopp_ce_MacAbstract, getThis(), "key", 3, key, keySize TSRMLS_CC);
}
/* }}} */

/* {{{ proto string MacAbstract::getKey(void)
   Returns the key */
PHP_METHOD(Cryptopp_MacAbstract, getKey) {
    zval *key = zend_read_property(cryptopp_ce_MacAbstract, getThis(), "key", 3, 1 TSRMLS_CC);
    RETURN_ZVAL(key, 1, 0)
}
/* }}} */

/* {{{ proto string MacAbstract::calculateDigest(string data)
   Calculate the MAC */
PHP_METHOD(Cryptopp_MacAbstract, calculateDigest) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::MessageAuthenticationCode *mac;
    CRYPTOPP_MAC_ABSTRACT_GET_NATIVE_PTR(mac)

    if (!isCryptoppMacKeyValid(getThis(), mac TSRMLS_CC)) {
        RETURN_FALSE;
    }

    byte digest[mac->DigestSize()];

    try {
        mac->CalculateDigest(digest, reinterpret_cast<byte*>(msg), msgSize);
    } catch (bool e) {
        RETURN_FALSE;
    }

    RETVAL_STRINGL(reinterpret_cast<char*>(digest), mac->DigestSize(), 1);
}
/* }}} */

/* {{{ proto void MacAbstract::update(string data)
   Adds data to the current incremental MAC */
PHP_METHOD(Cryptopp_MacAbstract, update) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::MessageAuthenticationCode *mac;
    CRYPTOPP_MAC_ABSTRACT_GET_NATIVE_PTR(mac)

    if (!isCryptoppMacKeyValid(getThis(), mac TSRMLS_CC)) {
        RETURN_FALSE;
    }

    mac->Update(reinterpret_cast<byte*>(msg), msgSize);
}
/* }}} */

/* {{{ proto string MacAbstract::finalize(void)
   Finalize an incremental MAC and return resulting digest */
PHP_METHOD(Cryptopp_MacAbstract, finalize) {
    CryptoPP::MessageAuthenticationCode *mac;
    CRYPTOPP_MAC_ABSTRACT_GET_NATIVE_PTR(mac)

    if (!isCryptoppMacKeyValid(getThis(), mac TSRMLS_CC)) {
        RETURN_FALSE;
    }

    byte digest[mac->DigestSize()];

    try {
        mac->Final(digest);
    } catch (bool e) {
        RETURN_FALSE;
    }

    RETVAL_STRINGL(reinterpret_cast<char*>(digest), mac->DigestSize(), 1);
}
/* }}} */

/* {{{ proto void MacAbstract::restart(void)
   Discard the current incremental MAC */
PHP_METHOD(Cryptopp_MacAbstract, restart) {
    CryptoPP::MessageAuthenticationCode *mac;
    CRYPTOPP_MAC_ABSTRACT_GET_NATIVE_PTR(mac)
    mac->Restart();
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
