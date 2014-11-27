#include "../php_cryptopp.h"
#include "php_mac_interface.h"
#include <zend_exceptions.h>

/* {{{ custom object create/free handler */
zend_object_handlers MacInterface_object_handlers;

void MacInterface_free_storage(void *object TSRMLS_DC) {
    MacInterfaceContainer *obj = static_cast<MacInterfaceContainer *>(object);
    delete obj->mac;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}

zend_object_value MacInterface_create_handler(zend_class_entry *type TSRMLS_DC) {
    zend_object_value retval;

    MacInterfaceContainer *obj = static_cast<MacInterfaceContainer*>(emalloc(sizeof(MacInterfaceContainer)));
    memset(obj, 0, sizeof(MacInterfaceContainer));

    zend_object_std_init(&obj->std, type TSRMLS_CC);

    #if PHP_VERSION_ID < 50399
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    #else
        object_properties_init(&obj->std, type);
    #endif

    retval.handle   = zend_objects_store_put(obj, NULL, MacInterface_free_storage, NULL TSRMLS_CC);
    retval.handlers = &MacInterface_object_handlers;

    return retval;
}

static int MacInternalInterface_implement_handler(zend_class_entry *interface, zend_class_entry *implementor TSRMLS_DC) {
    // MacInternalInterface cannot be implemented by a user class unless it extends an internal Hash class
    do {
        if (ZEND_INTERNAL_CLASS == implementor->type) {
            return SUCCESS;
        }

        implementor = implementor->parent;
    } while (NULL != implementor);

    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"MacInternalInterface cannot be implemented by user classes");
}
/* }}} */

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_MacInterface;
zend_class_entry *cryptopp_ce_MacInternalInterface;

static zend_function_entry cryptopp_methods_MacInterface[] = {
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, getName, arginfo_MacInterface_getName)
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, getDigestSize, arginfo_MacInterface_getDigestSize)
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, calculateDigest, arginfo_MacInterface_calculateDigest)
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, update, arginfo_MacInterface_update)
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, final, arginfo_MacInterface_final)
    PHP_ABSTRACT_ME(Cryptopp_MacInterface, restart, arginfo_MacInterface_restart)
    PHP_FE_END
};

static zend_function_entry cryptopp_methods_MacInternalInterface[] = {
    PHP_FE_END
};

void init_interface_MacInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "MacInterface", cryptopp_methods_MacInterface);
    cryptopp_ce_MacInterface = zend_register_internal_interface(&ce TSRMLS_CC);

    zend_class_entry ceInternal;
    INIT_NS_CLASS_ENTRY(ceInternal, "Cryptopp", "MacInternalInterface", cryptopp_methods_MacInternalInterface);
    cryptopp_ce_MacInternalInterface                                = zend_register_internal_interface(&ceInternal TSRMLS_CC);
    cryptopp_ce_MacInternalInterface->interface_gets_implemented    = MacInternalInterface_implement_handler;
}
/* }}} */

static zval *getKey(zval *object);
static bool ensureKeyIsValid(int keySize, CryptoPP::MessageAuthenticationCode *mac, zval *object);

/* {{{ verify that the constructor has been called */
static bool checkIfConstructorCalled(CryptoPP::MessageAuthenticationCode *mac) {
    if (NULL == mac) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Parent constructor was not called");
        return false;
    }

    return true;
}
/* }}} */

/* {{{ common implementation of MacInterface::setKey() */
void MacInterface_setKey(INTERNAL_FUNCTION_PARAMETERS) {
    char *key   = NULL;
    int keySize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &keySize)) {
        return;
    }

    CryptoPP::MessageAuthenticationCode *mac;
    mac = CRYPTOPP_MAC_GET_NATIVE_PTR();

    if (false == checkIfConstructorCalled(mac)) {
        RETURN_FALSE;
    }

    // ensure that the key is valid
    if (!ensureKeyIsValid(keySize, mac, getThis())) {
        return;
    }

    // set the key on both the php object and the native cryptopp object
    mac->SetKey(reinterpret_cast<byte*>(key), keySize);
    mac->Restart();
    zend_update_property_stringl(zend_get_class_entry(getThis() TSRMLS_CC), getThis(), "key", 3, key, keySize TSRMLS_CC);
}
/* }}} */

/* {{{ common implementation of MacInterface::calculateDigest() */
void MacInterface_calculateDigest(INTERNAL_FUNCTION_PARAMETERS) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::MessageAuthenticationCode *mac;
    mac = CRYPTOPP_MAC_GET_NATIVE_PTR();

    if (false == checkIfConstructorCalled(mac)) {
        RETURN_FALSE;
    }

    // ensure that the key is valid
    zval *key = getKey(getThis());

    if (!ensureKeyIsValid(Z_STRLEN_P(key), mac, getThis())) {
        return;
    }

    // calculate the digest
    byte digest[mac->DigestSize()];
    mac->CalculateDigest(digest, reinterpret_cast<byte*>(msg), msgSize);

    RETVAL_STRINGL(reinterpret_cast<char*>(digest), mac->DigestSize(), 1);
}
/* }}} */

/* {{{ common implementation of MacInterface::update() */
void MacInterface_update(INTERNAL_FUNCTION_PARAMETERS) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::MessageAuthenticationCode *mac;
    mac = CRYPTOPP_MAC_GET_NATIVE_PTR();

    if (false == checkIfConstructorCalled(mac)) {
        RETURN_FALSE;
    }

    // ensure that the key is valid
    zval *key = getKey(getThis());

    if (!ensureKeyIsValid(Z_STRLEN_P(key), mac, getThis())) {
        return;
    }

    // perform the update
    mac->Update(reinterpret_cast<byte*>(msg), msgSize);
}
/* }}} */

/* {{{ common implementation of MacInterface::final() */
void MacInterface_final(INTERNAL_FUNCTION_PARAMETERS) {
    CryptoPP::MessageAuthenticationCode *mac;
    mac = CRYPTOPP_MAC_GET_NATIVE_PTR();

    if (false == checkIfConstructorCalled(mac)) {
        RETURN_FALSE;
    }

    // ensure that the key is valid
    zval *key = getKey(getThis());

    if (!ensureKeyIsValid(Z_STRLEN_P(key), mac, getThis())) {
        return;
    }

    // calculate the digest
    byte digest[mac->DigestSize()];
    mac->Final(digest);

    RETVAL_STRINGL(reinterpret_cast<char*>(digest), mac->DigestSize(), 1);
}
/* }}} */

/* {{{ common implementation of MacInterface::restart() */
void MacInterface_restart(INTERNAL_FUNCTION_PARAMETERS) {
    CryptoPP::MessageAuthenticationCode *mac;
    mac = CRYPTOPP_MAC_GET_NATIVE_PTR();

    if (false == checkIfConstructorCalled(mac)) {
        RETURN_FALSE;
    }

    mac->Restart();
}
/* }}} */

/* {{{ common implementation of MacInterface::verify() */
void MacInterface_verify(INTERNAL_FUNCTION_PARAMETERS) {
    char *digest1   = NULL;
    char *digest2   = NULL;
    int digest1Size = 0;
    int digest2Size = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &digest1, &digest1Size, &digest2, &digest2Size)) {
        return;
    }

    // if one of the two digests is empty, or if their sizes does not match, return false
    if (0 == digest1Size || 0 == digest2Size || digest1Size != digest2Size) {
        RETURN_FALSE
    }

    // compare the digests
    int status = 0;

    for (int i = 0; i < digest1Size; i++) {
        status |= digest1[i] ^ digest2[i];
    }

    if (0 == status) {
        RETURN_TRUE
    } else {
        RETURN_FALSE
    }
}
/* }}} */

/* {{{ return the key of a MacInterface instance */
static zval *getKey(zval *object) {
    zend_class_entry *ce;
    zval *key;
    ce  = zend_get_class_entry(object TSRMLS_CC);
    key = zend_read_property(ce, object, "key", 3, 1 TSRMLS_CC);

    return key;
}
/* }}} */

/* {{{ ensure that a key size is valid for a MacInterface instance */
static bool ensureKeyIsValid(int keySize, CryptoPP::MessageAuthenticationCode *mac, zval *object) {
    if (!mac->IsValidKeyLength(keySize)) {
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
