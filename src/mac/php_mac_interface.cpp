#include "../php_cryptopp.h"
#include "php_mac_interface.h"

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_MacInterface;

static zend_function_entry cryptopp_methods_MacInterface[] = {
    PHP_ABSTRACT_ME(PHP_CRYPTOPP_NAMESPACE_MacInterface, getName, arginfo_MacInterface_getName)
    PHP_ABSTRACT_ME(PHP_CRYPTOPP_NAMESPACE_MacInterface, getDigestSize, arginfo_MacInterface_getDigestSize)
    PHP_ABSTRACT_ME(PHP_CRYPTOPP_NAMESPACE_MacInterface, calculateDigest, arginfo_MacInterface_calculateDigest)
    PHP_ABSTRACT_ME(PHP_CRYPTOPP_NAMESPACE_MacInterface, update, arginfo_MacInterface_update)
    PHP_ABSTRACT_ME(PHP_CRYPTOPP_NAMESPACE_MacInterface, final, arginfo_MacInterface_final)
    PHP_ABSTRACT_ME(PHP_CRYPTOPP_NAMESPACE_MacInterface, restart, arginfo_MacInterface_restart)
    PHP_FE_END
};

void init_interface_MacInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, PHP_CRYPTOPP_NAMESPACE, "MacInterface", cryptopp_methods_MacInterface);
    cryptopp_ce_MacInterface = zend_register_internal_interface(&ce TSRMLS_CC);
}
/* }}} */

/* {{{ custom object create/free handler */
zend_object_handlers MacInterface_object_handlers;

void MacInterface_free_storage(void *object TSRMLS_DC) {
    MacInterfaceContainer *obj = (MacInterfaceContainer *) object;
    delete obj->mac;
    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);
    efree(obj);
}

zend_object_value MacInterface_create_handler(zend_class_entry *type TSRMLS_DC) {
    zval *tmp;
    zend_object_value retval;

    MacInterfaceContainer *obj = (MacInterfaceContainer *) emalloc(sizeof(MacInterfaceContainer));
    memset(obj, 0, sizeof(MacInterfaceContainer));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);

    #if PHP_VERSION_ID < 50399
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    #else
        object_properties_init((zend_object*) &(obj->std), type);
    #endif

    retval.handle   = zend_objects_store_put(obj, NULL, MacInterface_free_storage, NULL TSRMLS_CC);
    retval.handlers = &MacInterface_object_handlers;

    return retval;
}
/* }}} */

/* common implementation of MacInterface::calculateDigest() */
void MacInterface_calculateDigest(INTERNAL_FUNCTION_PARAMETERS) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::MessageAuthenticationCode *mac;
    mac = CRYPTOPP_MAC_GET_NATIVE_PTR(classname);

    byte digest[mac->DigestSize()];
    mac->CalculateDigest(digest, (byte*) msg, msgSize);

    RETVAL_STRINGL((char*) digest, mac->DigestSize(), 1);
}

/* common implementation of MacInterface::update() */
void MacInterface_update(INTERNAL_FUNCTION_PARAMETERS) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::MessageAuthenticationCode *mac;
    mac = CRYPTOPP_MAC_GET_NATIVE_PTR(classname);

    mac->Update((byte*) msg, msgSize);
}

/* common implementation of MacInterface::final() */
void MacInterface_final(INTERNAL_FUNCTION_PARAMETERS) {
    CryptoPP::MessageAuthenticationCode *mac;
    mac = CRYPTOPP_MAC_GET_NATIVE_PTR(classname);

    byte digest[mac->DigestSize()];
    mac->Final(digest);

    RETVAL_STRINGL((char*) digest, mac->DigestSize(), 1);
}

/* common implementation of MacInterface::restart() */
void MacInterface_restart(INTERNAL_FUNCTION_PARAMETERS) {
    CryptoPP::MessageAuthenticationCode *mac;
    mac = CRYPTOPP_MAC_GET_NATIVE_PTR(classname);

    mac->Restart();
}
