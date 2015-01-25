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
#include "src/symmetric/cipher/php_symmetric_cipher_interface.h"
#include "php_block_cipher.h"
#include "php_block_cipher_interface.h"
#include "php_block_cipher_abstract.h"
#include <zend_exceptions.h>
#include <string>

/* {{{ custom object free handler */
void BlockCipherAbstract_free_storage(void *object TSRMLS_DC) {
    BlockCipherAbstractContainer *obj = static_cast<BlockCipherAbstractContainer *>(object);
    delete obj->encryptor;
    delete obj->decryptor;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}
/* }}} */

/* {{{ PHP abstract class declaration */
zend_object_handlers BlockCipherAbstract_object_handlers;
zend_class_entry *cryptopp_ce_BlockCipherAbstract;

static zend_function_entry cryptopp_methods_BlockCipherAbstract[] = {
    PHP_ME(Cryptopp_BlockCipherAbstract, __sleep, arginfo_BlockCipherAbstract___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_BlockCipherAbstract, __wakeup, arginfo_BlockCipherAbstract___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_BlockCipherAbstract, getName, arginfo_SymmetricCipherInterface_getName, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_BlockCipherAbstract, getBlockSize, arginfo_SymmetricCipherInterface_getBlockSize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_BlockCipherAbstract, isValidKeyLength, arginfo_SymmetricCipherInterface_isValidKeyLength, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_BlockCipherAbstract, setKey, arginfo_SymmetricCipherInterface_setKey, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_BlockCipherAbstract, getKey, arginfo_SymmetricCipherInterface_getKey, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_BlockCipherAbstract, encryptBlock, arginfo_BlockCipherInterface_encryptBlock, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_BlockCipherAbstract, decryptBlock, arginfo_BlockCipherInterface_decryptBlock, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_BlockCipherAbstract, encrypt, arginfo_BlockCipherInterface_encrypt, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_BlockCipherAbstract, decrypt, arginfo_BlockCipherInterface_decrypt, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_BlockCipherAbstract(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "BlockCipherAbstract", cryptopp_methods_BlockCipherAbstract);
    cryptopp_ce_BlockCipherAbstract                 = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_BlockCipherAbstract->ce_flags      |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

    cryptopp_ce_BlockCipherAbstract->create_object  = zend_custom_create_handler<BlockCipherAbstractContainer, BlockCipherAbstract_free_storage, &BlockCipherAbstract_object_handlers>;
    memcpy(&BlockCipherAbstract_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    BlockCipherAbstract_object_handlers.clone_obj   = NULL;

    zend_class_implements(cryptopp_ce_BlockCipherAbstract TSRMLS_CC, 1, cryptopp_ce_BlockCipherInterface);

    zend_declare_property_string(cryptopp_ce_BlockCipherAbstract, "name", 4, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_string(cryptopp_ce_BlockCipherAbstract, "key", 3, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ inits a child class */
void init_class_BlockCipherAbstractChild(const char *algoName, const char* className, zend_class_entry **classEntryPtr, zend_function_entry *classMethods TSRMLS_DC) {
    std::string namespacedClassName("Cryptopp\\");
    namespacedClassName.append(className);

    zend_class_entry ce;
    INIT_CLASS_ENTRY_EX(ce, namespacedClassName.c_str(), namespacedClassName.length(), classMethods);
    *classEntryPtr = zend_register_internal_class_ex(&ce, cryptopp_ce_BlockCipherAbstract, NULL TSRMLS_CC);

    blockCipherAlgoList.addAlgo(algoName, namespacedClassName.c_str());
}
/* }}} */

/* {{{ get the pointer to the native encryptor object of a php cipher class */
CryptoPP::BlockCipher *getCryptoppBlockCipherEncryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::BlockCipher *encryptor = static_cast<BlockCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->encryptor;

    if (NULL == encryptor) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\BlockCipherAbstract cannot be extended by user classes");
    }

    return encryptor;
}
/* }}} */

/* {{{ get the pointer to the native decryptor object of a php cipher class */
CryptoPP::BlockCipher *getCryptoppBlockCipherDecryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::BlockCipher *decryptor = static_cast<BlockCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->decryptor;

    if (NULL == decryptor) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\BlockCipherAbstract cannot be extended by user classes");
    }

    return decryptor;
}
/* }}} */

/* {{{ set the pointer to the native encryptor object of a php cipher class */
void setCryptoppBlockCipherEncryptorPtr(zval *this_ptr, CryptoPP::BlockCipher *encryptorPtr TSRMLS_DC) {
    static_cast<BlockCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->encryptor = encryptorPtr;
}
/* }}} */

/* {{{ set the pointer to the native decryptor object of a php cipher class */
void setCryptoppBlockCipherDecryptorPtr(zval *this_ptr, CryptoPP::BlockCipher *decryptorPtr TSRMLS_DC) {
    static_cast<BlockCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->decryptor = decryptorPtr;
}
/* }}} */

/* {{{ verify that a key size is valid for a BlockCipherAbstract instance */
bool isCryptoppBlockCipherKeyValid(zval *object, CryptoPP::BlockCipher *cipher TSRMLS_DC) {
    zval *key   = zend_read_property(cryptopp_ce_BlockCipherAbstract, object, "key", 3, 1 TSRMLS_CC);
    int keySize = Z_STRLEN_P(key);

    return isCryptoppSymmetricKeyValid(object, cipher, keySize TSRMLS_CC);
}
/* }}} */

/* {{{ proto void BlockCipherAbstract::__sleep(void)
   Prevents serialization of a BlockCipherAbstract instance */
PHP_METHOD(Cryptopp_BlockCipherAbstract, __sleep) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\BlockCipherAbstract instances");
}
/* }}} */

/* {{{ proto void BlockCipherAbstract::__wakeup(void)
   Prevents use of a BlockCipherAbstract instance that has been unserialized */
PHP_METHOD(Cryptopp_BlockCipherAbstract, __wakeup) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\BlockCipherAbstract instances");
}
/* }}} */

/* {{{ proto string BlockCipherAbstract::getName(void)
   Return algorithm name */
PHP_METHOD(Cryptopp_BlockCipherAbstract, getName) {
    CryptoPP::BlockCipher *encryptor    = CRYPTOPP_BLOCK_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)
    zval *name                          = zend_read_property(cryptopp_ce_BlockCipherAbstract, getThis(), "name", 4, 0 TSRMLS_CC);
    RETURN_ZVAL(name, 1, 0);
}
/* }}} */

/* {{{ proto int BlockCipherAbstract::getBlockSize(void)
   Returns the block size */
PHP_METHOD(Cryptopp_BlockCipherAbstract, getBlockSize) {
    CryptoPP::BlockCipher *encryptor;
    CRYPTOPP_BLOCK_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)
    RETURN_LONG(encryptor->BlockSize())
}
/* }}} */

/* {{{ proto bool BlockCipherAbstract::isValidKeyLength(int keyLength)
   Indicates if a key length is valid */
PHP_METHOD(Cryptopp_BlockCipherAbstract, isValidKeyLength) {
    long keySize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &keySize)) {
        return;
    }

    CryptoPP::BlockCipher *encryptor;
    CRYPTOPP_BLOCK_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)

    if (encryptor->IsValidKeyLength(keySize)) {
        RETURN_TRUE
    } else {
        RETURN_FALSE
    }
}
/* }}} */

/* {{{ proto void BlockCipherAbstract::setKey(string key)
   Sets the key */
PHP_METHOD(Cryptopp_BlockCipherAbstract, setKey) {
    char *key   = NULL;
    int keySize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &keySize)) {
        return;
    }

    CryptoPP::BlockCipher *encryptor;
    CryptoPP::BlockCipher *decryptor;
    CRYPTOPP_BLOCK_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)
    CRYPTOPP_BLOCK_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor)

    if (!isCryptoppSymmetricKeyValid(getThis(), encryptor, keySize TSRMLS_CC)) {
        RETURN_FALSE;
    }

    // set the key on both the php object and the native cryptopp objects
    byte *bKey = reinterpret_cast<byte*>(key);
    encryptor->SetKey(bKey, keySize);
    decryptor->SetKey(bKey, keySize);
    zend_update_property_stringl(cryptopp_ce_BlockCipherAbstract, getThis(), "key", 3, key, keySize TSRMLS_CC);
}
/* }}} */

/* {{{ proto string BlockCipherAbstract::getKey(void)
   Returns the key */
PHP_METHOD(Cryptopp_BlockCipherAbstract, getKey) {
    zval *key = zend_read_property(cryptopp_ce_BlockCipherAbstract, getThis(), "key", 3, 1 TSRMLS_CC);
    RETURN_ZVAL(key, 1, 0)
}
/* }}} */

/* {{{ proto string BlockCipherAbstract::encryptBlock(string block)
   Encrypts a single block of data */
PHP_METHOD(Cryptopp_BlockCipherAbstract, encryptBlock) {
    char *block     = NULL;
    int blockSize   = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &block, &blockSize)) {
        return;
    }

    CryptoPP::BlockCipher *encryptor;
    CRYPTOPP_BLOCK_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)

    // check key
    if (!isCryptoppBlockCipherKeyValid(getThis(), encryptor TSRMLS_CC)) {
        RETURN_FALSE
    }

    // check block size
    if (encryptor->BlockSize() != blockSize) {
        zend_class_entry *ce = zend_get_class_entry(getThis() TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s: data size (%d) is not equal to cipher block size (%d)", ce->name, blockSize, encryptor->BlockSize());
        RETURN_FALSE
    }

    // encrypt
    byte output[blockSize];
    encryptor->ProcessAndXorBlock(reinterpret_cast<byte*>(block), NULL, output);

    RETURN_STRINGL(reinterpret_cast<char*>(output), blockSize, 1)
}
/* }}} */

/* {{{ proto string BlockCipherAbstract::decryptBlock(string block)
   Decrypts a single block of data */
PHP_METHOD(Cryptopp_BlockCipherAbstract, decryptBlock) {
    char *block     = NULL;
    int blockSize   = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &block, &blockSize)) {
        return;
    }

    CryptoPP::BlockCipher *decryptor;
    CRYPTOPP_BLOCK_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor)

    // check key
    if (!isCryptoppBlockCipherKeyValid(getThis(), decryptor TSRMLS_CC)) {
        RETURN_FALSE
    }

    // check block size
    if (decryptor->BlockSize() != blockSize) {
        zend_class_entry *ce = zend_get_class_entry(getThis() TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s: data size (%d) is not equal to cipher block size (%d)", ce->name, blockSize, decryptor->BlockSize());
        RETURN_FALSE
    }

    // decrypt
    byte output[blockSize];
    decryptor->ProcessAndXorBlock(reinterpret_cast<byte*>(block), NULL, output);

    RETURN_STRINGL(reinterpret_cast<char*>(output), blockSize, 1)
}
/* }}} */

/* {{{ proto string BlockCipherAbstract::encrypt(string data)
   Encrypts data */
PHP_METHOD(Cryptopp_BlockCipherAbstract, encrypt) {
    char *data      = NULL;
    int dataSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &dataSize)) {
        return;
    }

    CryptoPP::BlockCipher *encryptor;
    CRYPTOPP_BLOCK_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)

    // check key
    if (!isCryptoppBlockCipherKeyValid(getThis(), encryptor TSRMLS_CC)) {
        RETURN_FALSE
    }

    // check dataSize against block size
    int blockSize = static_cast<int>(encryptor->BlockSize());

    if (0 != dataSize % blockSize) {
        zend_class_entry *ce = zend_get_class_entry(getThis() TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s: data size (%d) is not a multiple of block size (%d)", ce->name, dataSize, blockSize);
        RETURN_FALSE
    }

    // encrypt
    byte *input     = reinterpret_cast<byte*>(data);
    byte *output    = new byte[dataSize];
    byte block[blockSize];
    int blocks = dataSize / blockSize;

    for (int i = 0; i < blocks; i++) {
        encryptor->ProcessAndXorBlock(&input[i * blockSize], NULL, &output[i * blockSize]);
    }

    RETVAL_STRINGL(reinterpret_cast<char*>(output), dataSize, 1);
    delete[] output;
}
/* }}} */

/* {{{ proto string BlockCipherAbstract::decrypt(string data)
   Decrypts data */
PHP_METHOD(Cryptopp_BlockCipherAbstract, decrypt) {
    char *data      = NULL;
    int dataSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &dataSize)) {
        return;
    }

    CryptoPP::BlockCipher *decryptor;
    CRYPTOPP_BLOCK_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor)

    // check key
    if (!isCryptoppBlockCipherKeyValid(getThis(), decryptor TSRMLS_CC)) {
        RETURN_FALSE
    }

    // check dataSize against block size
    int blockSize = static_cast<int>(decryptor->BlockSize());

    if (0 != dataSize % blockSize) {
        zend_class_entry *ce = zend_get_class_entry(getThis() TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s: data size (%d) is not a multiple of block size (%d)", ce->name, dataSize, blockSize);
        RETURN_FALSE
    }

    // encrypt
    byte *input     = reinterpret_cast<byte*>(data);
    byte *output    = new byte[dataSize];
    byte block[blockSize];
    int blocks = dataSize / blockSize;

    for (int i = 0; i < blocks; i++) {
        decryptor->ProcessAndXorBlock(&input[i * blockSize], NULL, &output[i * blockSize]);
    }

    RETVAL_STRINGL(reinterpret_cast<char*>(output), dataSize, 1);
    delete[] output;
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
