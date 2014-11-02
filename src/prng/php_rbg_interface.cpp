#include "../php_cryptopp.h"
#include "php_rbg_interface.h"
#include "../exception/php_exception.h"
#include <string>
#include <zend_exceptions.h>

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_RandomByteGeneratorInterface;

static zend_function_entry cryptopp_methods_RandomByteGeneratorInterface[] = {
    PHP_ABSTRACT_ME(PHP_CRYPTOPP_NAMESPACE_RandomByteGeneratorInterface, generate, arginfo_RandomByteGeneratorInterface_generate)
    PHP_FE_END
};

void init_interface_RandomByteGeneratorInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, PHP_CRYPTOPP_NAMESPACE, "RandomByteGeneratorInterface", cryptopp_methods_RandomByteGeneratorInterface);
    cryptopp_ce_RandomByteGeneratorInterface = zend_register_internal_interface(&ce TSRMLS_CC);
}
/* }}} */

/* {{{ custom object create/free handler */
zend_object_handlers RandomByteGeneratorInterface_object_handlers;

void RandomByteGeneratorInterface_free_storage(void *object TSRMLS_DC) {
    RandomByteGeneratorInterfaceContainer *obj = (RandomByteGeneratorInterfaceContainer *) object;
    delete obj->rbg;
    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);
    efree(obj);
}

zend_object_value RandomByteGeneratorInterface_create_handler(zend_class_entry *type TSRMLS_DC) {
    zval *tmp;
    zend_object_value retval;

    RandomByteGeneratorInterfaceContainer *obj = (RandomByteGeneratorInterfaceContainer *) emalloc(sizeof(RandomByteGeneratorInterfaceContainer));
    memset(obj, 0, sizeof(RandomByteGeneratorInterfaceContainer));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);

    #if PHP_VERSION_ID < 50399
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    #else
        object_properties_init((zend_object*) &(obj->std), type);
    #endif

    retval.handle   = zend_objects_store_put(obj, NULL, RandomByteGeneratorInterface_free_storage, NULL TSRMLS_CC);
    retval.handlers = &RandomByteGeneratorInterface_object_handlers;

    return retval;
}
/* }}} */

// TODO test serialize
// TODO test clone
/* common implementation of RandomByteGeneratorInterface::generate() */
void RandomByteGeneratorInterface_generate(INTERNAL_FUNCTION_PARAMETERS) {
    long size = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &size)) {
        return;
    }

    // verify that size > 0
    if (size <= 0) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, "Size must be a positive integer, %ld given", size);
        RETURN_FALSE;
    }

    // generate random bytes
    CryptoPP::RandomNumberGenerator *rbg;
    rbg = CRYPTOPP_RBG_GET_NATIVE_PTR(classname);

    byte block[size];
    rbg->GenerateBlock(block, size);

    RETVAL_STRINGL((char*) block, size, 1);
}
