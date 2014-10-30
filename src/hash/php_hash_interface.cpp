#include "../php_cryptopp.h"
#include "php_hash_interface.h"
#include <string>

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_HashInterface;

static zend_function_entry cryptopp_methods_HashInterface[] = {
    PHP_ABSTRACT_ME(PHP_CRYPTOPP_NAMESPACE_HashInterface, getName, arginfo_HashInterface_getName)
    PHP_ABSTRACT_ME(PHP_CRYPTOPP_NAMESPACE_HashInterface, getDigestSize, arginfo_HashInterface_getDigestSize)
    PHP_ABSTRACT_ME(PHP_CRYPTOPP_NAMESPACE_HashInterface, calculateDigest, arginfo_HashInterface_calculateDigest)
    PHP_FE_END
};

void init_interface_HashInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, PHP_CRYPTOPP_NAMESPACE, "HashInterface", cryptopp_methods_HashInterface);
    cryptopp_ce_HashInterface = zend_register_internal_interface(&ce TSRMLS_CC);
}
/* }}} */

/* {{{ custom object create/free handler */
zend_object_handlers HashInterface_object_handlers;

void HashInterface_free_storage(void *object TSRMLS_DC) {
    HashInterfaceContainer *obj = (HashInterfaceContainer *) object;
    delete obj->hash;
    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);
    efree(obj);
}

zend_object_value HashInterface_create_handler(zend_class_entry *type TSRMLS_DC) {
    zval *tmp;
    zend_object_value retval;

    HashInterfaceContainer *obj = (HashInterfaceContainer *) emalloc(sizeof(HashInterfaceContainer));
    memset(obj, 0, sizeof(HashInterfaceContainer));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);

    if (PHP_VERSION_ID < 50399) {
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    } else {
        object_properties_init((zend_object*) &(obj->std), type);
    }

    retval.handle   = zend_objects_store_put(obj, NULL, HashInterface_free_storage, NULL TSRMLS_CC);
    retval.handlers = &HashInterface_object_handlers;

    return retval;
}
/* }}} */

/* common implementation of HashInterface::calculateDigest() */
void HashInterface_calculateDigest(INTERNAL_FUNCTION_PARAMETERS) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::HashTransformation *hash;
    hash = CRYPTOPP_HASH_GET_NATIVE_PTR(classname);

    byte digest[hash->DigestSize()];
    hash->CalculateDigest(digest, (byte*) msg, msgSize);

    RETVAL_STRINGL((char*) digest, hash->DigestSize(), 1);
}

/* common implementation of HashInterface::update() */
void HashInterface_update(INTERNAL_FUNCTION_PARAMETERS) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::HashTransformation *hash;
    hash = CRYPTOPP_HASH_GET_NATIVE_PTR(classname);

    hash->Update((byte*) msg, msgSize);
}

/* common implementation of HashInterface::final() */
void HashInterface_final(INTERNAL_FUNCTION_PARAMETERS) {
    CryptoPP::HashTransformation *hash;
    hash = CRYPTOPP_HASH_GET_NATIVE_PTR(classname);

    byte digest[hash->DigestSize()];
    hash->Final(digest);

    RETVAL_STRINGL((char*) digest, hash->DigestSize(), 1);
}

/* common implementation of HashInterface::restart() */
void HashInterface_restart(INTERNAL_FUNCTION_PARAMETERS) {
    CryptoPP::HashTransformation *hash;
    hash = CRYPTOPP_HASH_GET_NATIVE_PTR(classname);

    hash->Restart();
}
