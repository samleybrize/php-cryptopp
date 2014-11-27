#include "../../php_cryptopp.h"
#include "php_symmetric_cipher_interface.h"
#include <zend_exceptions.h>
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

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
