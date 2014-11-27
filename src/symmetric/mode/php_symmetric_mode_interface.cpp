#include "../../php_cryptopp.h"
#include "php_symmetric_mode_interface.h"
#include <zend_exceptions.h>
#include <string>

/* {{{ custom object create/free handler */
zend_object_handlers SymmetricModeInterface_object_handlers;

void SymmetricModeInterface_free_storage(void *object TSRMLS_DC) {
    SymmetricModeInterfaceContainer *obj = static_cast<SymmetricModeInterfaceContainer *>(object);
    delete obj->encryptor;
    delete obj->decryptor;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}

zend_object_value SymmetricModeInterface_create_handler(zend_class_entry *type TSRMLS_DC) {
    zend_object_value retval;

    SymmetricModeInterfaceContainer *obj = static_cast<SymmetricModeInterfaceContainer *>(emalloc(sizeof(SymmetricModeInterfaceContainer)));
    memset(obj, 0, sizeof(SymmetricModeInterfaceContainer));

    zend_object_std_init(&obj->std, type TSRMLS_CC);

    #if PHP_VERSION_ID < 50399
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    #else
        object_properties_init(static_cast<zend_object*>(&(obj->std)), type);
    #endif

    retval.handle   = zend_objects_store_put(obj, NULL, SymmetricModeInterface_free_storage, NULL TSRMLS_CC);
    retval.handlers = &SymmetricModeInterface_object_handlers;

    return retval;
}

static int SymmetricModeInternalInterface_implement_handler(zend_class_entry *interface, zend_class_entry *implementor TSRMLS_DC) {
    // SymmetricModeInternalInterface cannot be implemented by a user class unless it extends an internal SymmetricMode class
    do {
        if (ZEND_INTERNAL_CLASS == implementor->type) {
            return SUCCESS;
        }

        implementor = implementor->parent;
    } while (NULL != implementor);

    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"SymmetricModeInternalInterface cannot be implemented by user classes");
}
/* }}} */

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_SymmetricModeInterface;
zend_class_entry *cryptopp_ce_SymmetricModeInternalInterface;

static zend_function_entry cryptopp_methods_SymmetricModeInterface[] = {
    PHP_ABSTRACT_ME(Cryptopp_SymmetricModeInterface, getName, arginfo_SymmetricModeInterface_getName)
    PHP_ABSTRACT_ME(Cryptopp_SymmetricModeInterface, setKey, arginfo_SymmetricModeInterface_setKey)
    PHP_FE_END
};

static zend_function_entry cryptopp_methods_SymmetricModeInternalInterface[] = {
    PHP_FE_END
};

void init_interface_SymmetricModeInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "SymmetricModeInterface", cryptopp_methods_SymmetricModeInterface);
    cryptopp_ce_SymmetricModeInterface = zend_register_internal_interface(&ce TSRMLS_CC);

    zend_class_entry ceInternal;
    INIT_NS_CLASS_ENTRY(ceInternal, "Cryptopp", "SymmetricModeInternalInterface", cryptopp_methods_SymmetricModeInternalInterface);
    cryptopp_ce_SymmetricModeInternalInterface                              = zend_register_internal_interface(&ceInternal TSRMLS_CC);
    cryptopp_ce_SymmetricModeInternalInterface->interface_gets_implemented  = SymmetricModeInternalInterface_implement_handler;
}
/* }}} */

/* {{{ return a pointer to the SymmetricModeInterface class entry */
zend_class_entry *getCryptoppSymmetricModeInterface() {
    return cryptopp_ce_SymmetricModeInterface;
}
/* }}} */

/* {{{ return a pointer to the SymmetricModeInternalInterface class entry */
zend_class_entry *getCryptoppSymmetricModeInternalInterface() {
    return cryptopp_ce_SymmetricModeInternalInterface;
}
/* }}} */

/* {{{ Get needed cipher elements to build a mode object */
bool cryptoppSymmetricModeGetCipherElements(
    zval *cipherObject,
    zval *modeObject,
    CryptoPP::BlockCipher **cipherEncryptor,
    CryptoPP::BlockCipher **cipherDecryptor,
    std::string **cipherName
) {
    // get cipher encryptor/decryptor ptr
    *cipherEncryptor = static_cast<SymmetricCipherInterfaceContainer *>(zend_object_store_get_object(cipherObject TSRMLS_CC))->encryptor;
    *cipherDecryptor = static_cast<SymmetricCipherInterfaceContainer *>(zend_object_store_get_object(cipherObject TSRMLS_CC))->decryptor;

    // verify that cipher encryptor/decryptor ptr are not null
    if (NULL == cipherEncryptor || NULL == cipherDecryptor) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(cipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : parent constructor was not called", ce->name);
        return false;
    }

    // retrieve the name of the cipher
    zval *zCipherName;
    zval *funcName;
    MAKE_STD_ZVAL(zCipherName);
    MAKE_STD_ZVAL(funcName);
    ZVAL_STRING(funcName, "getName", 1);
    call_user_function(NULL, &cipherObject, funcName, zCipherName, 0, NULL TSRMLS_CC);

    *cipherName = new std::string(Z_STRVAL_P(zCipherName), Z_STRLEN_P(zCipherName));

    zval_dtor(zCipherName);
    zval_dtor(funcName);

    return true;
}
/* }}} */

/* {{{ verify that the constructor has been called */
static bool checkIfConstructorCalled(CryptoPP::CipherModeBase *mac) {
    if (NULL == mac) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Parent constructor was not called");
        return false;
    }

    return true;
}
/* }}} */

/* {{{ ensure that a key size is valid for a SymmetricModeInterface instance */
static bool ensureKeyIsValid(int keySize, CryptoPP::CipherModeBase *mode, zval *object) {
    if (!mode->IsValidKeyLength(keySize)) {
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

/* {{{ common implementation of SymmetricModeInterface::setKey() */
void SymmetricModeInterface_setKey(INTERNAL_FUNCTION_PARAMETERS) {
    char *key   = NULL;
    int keySize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &keySize)) {
        return;
    }

    CryptoPP::CipherModeBase *encryptor;
    CryptoPP::CipherModeBase *decryptor;
    encryptor = CRYPTOPP_SYMMETRIC_MODE_GET_ENCRYPTOR_PTR();
    decryptor = CRYPTOPP_SYMMETRIC_MODE_GET_DECRYPTOR_PTR();

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

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
