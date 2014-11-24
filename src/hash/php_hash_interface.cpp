#include "../php_cryptopp.h"
#include "php_hash_interface.h"
#include <string>

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_HashInterface;

static zend_function_entry cryptopp_methods_HashInterface[] = {
    PHP_ABSTRACT_ME(Cryptopp_HashInterface, getName, arginfo_HashInterface_getName)
    PHP_ABSTRACT_ME(Cryptopp_HashInterface, getDigestSize, arginfo_HashInterface_getDigestSize)
    PHP_ABSTRACT_ME(Cryptopp_HashInterface, calculateDigest, arginfo_HashInterface_calculateDigest)
    PHP_ABSTRACT_ME(Cryptopp_HashInterface, update, arginfo_HashInterface_update)
    PHP_ABSTRACT_ME(Cryptopp_HashInterface, final, arginfo_HashInterface_final)
    PHP_ABSTRACT_ME(Cryptopp_HashInterface, restart, arginfo_HashInterface_restart)
    PHP_FE_END
};

void init_interface_HashInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "HashInterface", cryptopp_methods_HashInterface);
    cryptopp_ce_HashInterface = zend_register_internal_interface(&ce TSRMLS_CC);
}
/* }}} */

/* {{{ custom object create/free handler */
zend_object_handlers HashInterface_object_handlers;

void HashInterface_free_storage(void *object TSRMLS_DC) {
    HashInterfaceContainer *obj = static_cast<HashInterfaceContainer *>(object);
    delete obj->hash;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}

zend_object_value HashInterface_create_handler(zend_class_entry *type TSRMLS_DC) {
    zend_object_value retval;

    HashInterfaceContainer *obj = static_cast<HashInterfaceContainer *>(emalloc(sizeof(HashInterfaceContainer)));
    memset(obj, 0, sizeof(HashInterfaceContainer));

    zend_object_std_init(&obj->std, type TSRMLS_CC);

    #if PHP_VERSION_ID < 50399
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    #else
        object_properties_init(static_cast<zend_object*>(&(obj->std)), type);
    #endif

    retval.handle   = zend_objects_store_put(obj, NULL, HashInterface_free_storage, NULL TSRMLS_CC);
    retval.handlers = &HashInterface_object_handlers;

    return retval;
}
/* }}} */

/* {{{ return a pointer to the HashInterface class entry */
zend_class_entry *getCryptoppHashInterface() {
    return cryptopp_ce_HashInterface;
}
/* }}} */

/* {{{ verify that the constructor has been called */
static bool checkIfConstructorCalled(CryptoPP::HashTransformation *hash) {
    if (NULL == hash) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Constructor was not called");
        return false;
    }

    return true;
}
/* }}} */

/* {{{ common implementation of HashInterface::calculateDigest() */
void HashInterface_calculateDigest(INTERNAL_FUNCTION_PARAMETERS) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::HashTransformation *hash;
    hash = CRYPTOPP_HASH_GET_NATIVE_PTR();

    if (false == checkIfConstructorCalled(hash)) {
        RETURN_FALSE;
    }

    byte digest[hash->DigestSize()];
    hash->CalculateDigest(digest, reinterpret_cast<byte*>(msg), msgSize);

    RETVAL_STRINGL(reinterpret_cast<char*>(digest), hash->DigestSize(), 1);
}
/* }}} */

/* {{{ common implementation of HashInterface::update() */
void HashInterface_update(INTERNAL_FUNCTION_PARAMETERS) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::HashTransformation *hash;
    hash = CRYPTOPP_HASH_GET_NATIVE_PTR();

    if (false == checkIfConstructorCalled(hash)) {
        RETURN_FALSE;
    }

    hash->Update(reinterpret_cast<byte*>(msg), msgSize);
}
/* }}} */

/* {{{ common implementation of HashInterface::final() */
void HashInterface_final(INTERNAL_FUNCTION_PARAMETERS) {
    CryptoPP::HashTransformation *hash;
    hash = CRYPTOPP_HASH_GET_NATIVE_PTR();

    if (false == checkIfConstructorCalled(hash)) {
        RETURN_FALSE;
    }

    byte digest[hash->DigestSize()];
    hash->Final(digest);

    RETVAL_STRINGL(reinterpret_cast<char*>(digest), hash->DigestSize(), 1);
}
/* }}} */

/* {{{ common implementation of HashInterface::restart() */
void HashInterface_restart(INTERNAL_FUNCTION_PARAMETERS) {
    CryptoPP::HashTransformation *hash;
    hash = CRYPTOPP_HASH_GET_NATIVE_PTR();

    if (false == checkIfConstructorCalled(hash)) {
        RETURN_FALSE;
    }

    hash->Restart();
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
