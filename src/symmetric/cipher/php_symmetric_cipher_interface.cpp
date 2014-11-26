#include "../../php_cryptopp.h"
#include "php_symmetric_cipher_interface.h"
#include <zend_exceptions.h>
#include <math.h>
#include <string>

/* {{{ custom object create/free handler */
zend_object_handlers SymmetricCipherInterface_object_handlers;

void SymmetricCipherInterface_free_storage(void *object TSRMLS_DC) {
    SymmetricCipherInterfaceContainer *obj = static_cast<SymmetricCipherInterfaceContainer *>(object);
    delete obj->encryptor;
    delete obj->decryptor;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}

zend_object_value SymmetricCipherInterface_create_handler(zend_class_entry *type TSRMLS_DC) {
    zend_object_value retval;

    SymmetricCipherInterfaceContainer *obj = static_cast<SymmetricCipherInterfaceContainer *>(emalloc(sizeof(SymmetricCipherInterfaceContainer)));
    memset(obj, 0, sizeof(SymmetricCipherInterfaceContainer));

    zend_object_std_init(&obj->std, type TSRMLS_CC);

    #if PHP_VERSION_ID < 50399
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    #else
        object_properties_init(static_cast<zend_object*>(&(obj->std)), type);
    #endif

    retval.handle   = zend_objects_store_put(obj, NULL, SymmetricCipherInterface_free_storage, NULL TSRMLS_CC);
    retval.handlers = &SymmetricCipherInterface_object_handlers;

    return retval;
}

static int SymmetricCipherInternalInterface_implement_handler(zend_class_entry *interface, zend_class_entry *implementor TSRMLS_DC) {
    // SymmetricCipherInternalInterface cannot be implemented by a user class unless it extends an internal SymmetricCipher class
    do {
        if (ZEND_INTERNAL_CLASS == implementor->type) {
            return SUCCESS;
        }

        implementor = implementor->parent;
    } while (NULL != implementor);

    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"SymmetricCipherInternalInterface cannot be implemented by user classes");
}
/* }}} */

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_SymmetricCipherInterface;
zend_class_entry *cryptopp_ce_SymmetricCipherInternalInterface;

static zend_function_entry cryptopp_methods_SymmetricCipherInterface[] = {
    PHP_ABSTRACT_ME(Cryptopp_SymmetricCipherInterface, getName, arginfo_SymmetricCipherInterface_getName)
    PHP_FE_END
};

static zend_function_entry cryptopp_methods_SymmetricCipherInternalInterface[] = {
    PHP_FE_END
};

void init_interface_SymmetricCipherInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "SymmetricCipherInterface", cryptopp_methods_SymmetricCipherInterface);
    cryptopp_ce_SymmetricCipherInterface = zend_register_internal_interface(&ce TSRMLS_CC);

    zend_class_entry ceInternal;
    INIT_NS_CLASS_ENTRY(ceInternal, "Cryptopp", "SymmetricCipherInternalInterface", cryptopp_methods_SymmetricCipherInternalInterface);
    cryptopp_ce_SymmetricCipherInternalInterface                                = zend_register_internal_interface(&ceInternal TSRMLS_CC);
    cryptopp_ce_SymmetricCipherInternalInterface->interface_gets_implemented    = SymmetricCipherInternalInterface_implement_handler;
}
/* }}} */

/* {{{ return a pointer to the SymmetricCipherInterface class entry */
zend_class_entry *getCryptoppSymmetricCipherInterface() {
    return cryptopp_ce_SymmetricCipherInterface;
}
/* }}} */

/* {{{ return a pointer to the SymmetricCipherInternalInterface class entry */
zend_class_entry *getCryptoppSymmetricCipherInternalInterface() {
    return cryptopp_ce_SymmetricCipherInternalInterface;
}
/* }}} */

static zval *getKey(zval *object);
static bool ensureKeyIsValid(int keySize, CryptoPP::BlockCipher *cipher, zval *object);

/* {{{ verify that the constructor has been called */
static bool checkIfConstructorCalled(CryptoPP::BlockCipher *cipher) {
    if (NULL == cipher) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Constructor was not called");
        return false;
    }

    return true;
}
/* }}} */

/* {{{ common implementation of SymmetricCipherInterface::setKey() */
void SymmetricCipherInterface_setKey(INTERNAL_FUNCTION_PARAMETERS) {
    char *key   = NULL;
    int keySize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &keySize)) {
        return;
    }

    CryptoPP::BlockCipher *encryptor;
    CryptoPP::BlockCipher *decryptor;
    encryptor = CRYPTOPP_SYMMETRIC_CIPHER_GET_ENCRYPTOR_PTR();
    decryptor = CRYPTOPP_SYMMETRIC_CIPHER_GET_DECRYPTOR_PTR();

    if (false == checkIfConstructorCalled(encryptor)) {
        RETURN_FALSE;
    }

    // ensure that the key is valid
    if (!ensureKeyIsValid(keySize, encryptor, getThis())) {
        return;
    }

    // set the key on both the php object and the native cryptopp objects
    encryptor->SetKey(reinterpret_cast<byte*>(key), keySize);
    decryptor->SetKey(reinterpret_cast<byte*>(key), keySize);
    zend_update_property_stringl(zend_get_class_entry(getThis() TSRMLS_CC), getThis(), "key", 3, key, keySize TSRMLS_CC);
}
/* }}} */

/* {{{ common implementation of SymmetricCipherInterface::encrypt() */
void SymmetricCipherInterface_encrypt(INTERNAL_FUNCTION_PARAMETERS) {
    char *plain     = NULL;
    int plainSize   = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &plain, &plainSize)) {
        return;
    }

    CryptoPP::BlockCipher *encryptor;
    encryptor = CRYPTOPP_SYMMETRIC_CIPHER_GET_ENCRYPTOR_PTR();

    if (false == checkIfConstructorCalled(encryptor)) {
        RETURN_FALSE;
    }

    // ensure that the key is valid
    zval *key = getKey(getThis());

    if (!ensureKeyIsValid(Z_STRLEN_P(key), encryptor, getThis())) {
        return;
    }

    // calculate the ciphertext
    unsigned int blockSize  = encryptor->BlockSize();
    unsigned int blocks     = ceil(static_cast<double>(plainSize) / blockSize);
    unsigned int size       = blocks * blockSize;
    byte ciphertext[size];

    for (int i = 0; i < blocks; i++) {
        encryptor->ProcessBlock(reinterpret_cast<byte*>(&plain[i * blockSize]), &ciphertext[i * blockSize]);
    }

    RETVAL_STRINGL(reinterpret_cast<char*>(ciphertext), size, 1);
}
/* }}} */

/* {{{ common implementation of SymmetricCipherInterface::decrypt() */
void SymmetricCipherInterface_decrypt(INTERNAL_FUNCTION_PARAMETERS) {
    char *ciphertext    = NULL;
    int ciphertextSize  = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ciphertext, &ciphertextSize)) {
        return;
    }

    CryptoPP::BlockCipher *decryptor;
    decryptor = CRYPTOPP_SYMMETRIC_CIPHER_GET_DECRYPTOR_PTR();

    if (false == checkIfConstructorCalled(decryptor)) {
        RETURN_FALSE;
    }

    // ensure that the key is valid
    zval *key = getKey(getThis());

    if (!ensureKeyIsValid(Z_STRLEN_P(key), decryptor, getThis())) {
        return;
    }

    // calculate the ciphertext
    unsigned int blockSize  = decryptor->BlockSize();
    unsigned int blocks     = ceil(static_cast<double>(ciphertextSize) / blockSize);
    unsigned int size       = blocks * blockSize;
    byte plain[ciphertextSize];

    for (int i = 0; i < blocks; i++) {
        decryptor->ProcessBlock(reinterpret_cast<byte*>(&ciphertext[i * blockSize]), &plain[i * blockSize]);
    }

    RETVAL_STRINGL(reinterpret_cast<char*>(plain), ciphertextSize, 1);
}
/* }}} */

/* {{{ return the key of a SymmetricCipherInterface instance */
static zval *getKey(zval *object) {
    zend_class_entry *ce;
    zval *key;
    ce  = zend_get_class_entry(object TSRMLS_CC);
    key = zend_read_property(ce, object, "key", 3, 1 TSRMLS_CC);

    return key;
}
/* }}} */

/* {{{ ensure that a key size is valid for a SymmetricCipherInterface instance */
static bool ensureKeyIsValid(int keySize, CryptoPP::BlockCipher *cipher, zval *object) {
    if (!cipher->IsValidKeyLength(keySize)) {
        zend_class_entry *ce;
        ce = zend_get_class_entry(object TSRMLS_CC);

        if (0 == keySize) {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : a key is required", ce->name, keySize);
        } else {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : %d is not a valid key length", ce->name, keySize); // TODO indicates required key length
        }
        return false;
    }

    return true;
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
