#ifndef HASH_HASH_INTERFACE_H
#define HASH_HASH_INTERFACE_H

extern "C" {
#include "php.h"
}

// TODO type->properties_info ?????
#define CRYPTOPP_HASH_INIT_CLASS(classname, nativeClassname, classEntryPtrName, classMethodsVarName) \
    zend_object_handlers classname ## _object_handlers;                     \
    struct classname ## Container {                                         \
        zend_object std;                                                    \
        nativeClassname *hash;                                              \
    };                                                                      \
    void classname ## _free_storage(void *object TSRMLS_DC) {               \
        classname ## Container *obj = (classname ## Container *) object;    \
        delete obj->hash;                                                   \
        zend_hash_destroy(obj->std.properties);                             \
        FREE_HASHTABLE(obj->std.properties);                                \
        efree(obj);                                                         \
    }                                                                       \
    zend_object_value classname ## _create_handler(zend_class_entry *type TSRMLS_DC) { \
        zval *tmp;                                                          \
        zend_object_value retval;                                           \
                                                                            \
        classname ## Container *obj = (classname ## Container *) emalloc(sizeof(classname ## Container)); \
        memset(obj, 0, sizeof(classname ## Container));                     \
        obj->std.ce = type;                                                 \
                                                                            \
        ALLOC_HASHTABLE(obj->std.properties);                               \
        zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);     \
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *)); \
                                                                            \
        retval.handle   = zend_objects_store_put(obj, NULL, classname ## _free_storage, NULL TSRMLS_CC); \
        retval.handlers = &classname ## _object_handlers;                   \
                                                                            \
        return retval;                                                      \
    }                                                                       \
    CRYPTOPP_HASH_GET_INIT_CLASS_FUNC_HEADER(classname) {                   \
        zend_class_entry ce;                                                \
        INIT_CLASS_ENTRY(ce, #classname, classMethodsVarName);              \
        classEntryPtrName = zend_register_internal_class(&ce TSRMLS_CC);    \
                                                                            \
        classEntryPtrName->create_object = classname ## _create_handler;    \
        memcpy(&classname ## _object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
        classname ## _object_handlers.clone_obj = NULL;                     \
                                                                            \
        zend_class_implements(classEntryPtrName TSRMLS_CC, 1, cryptopp_ce_HashInterface); \
    }

#define CRYPTOPP_HASH_GET_INIT_CLASS_FUNC_HEADER(classname) void init_class_ ## classname(TSRMLS_D)
#define CRYPTOPP_HASH_GET_INIT_CLASS_FUNC_CALL(classname) init_class_ ## classname(TSRMLS_C);

#define CRYPTOPP_HASH_GET_NATIVE_PTR(classname) ((classname ## Container *)zend_object_store_get_object(getThis() TSRMLS_CC))->hash
#define CRYPTOPP_HASH_SET_NATIVE_PTR(classname, nativeHashPtr) ((classname ## Container *)zend_object_store_get_object(getThis() TSRMLS_CC))->hash = nativeHashPtr;

#define CRYPTOPP_HASH_GET_REQUIRED_METHODS(classname) \
    PHP_ME(classname, hash, NULL, ZEND_ACC_PUBLIC)

#define CRYPTOPP_HASH_GET_REQUIRED_METHODS_HEADER(classname) \
    PHP_METHOD(classname, hash);

#define CRYPTOPP_HASH_GET_REQUIRED_METHODS_DEFINITIONS(classname, nativeClassname)  \
    PHP_METHOD(classname, hash) {                                                   \
        char *msg   = NULL;                                                         \
        int msgSize = 0;                                                            \
                                                                                    \
        if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) { \
            return;                                                                 \
        }                                                                           \
                                                                                    \
        nativeClassname *hash;                                                      \
        hash = CRYPTOPP_HASH_GET_NATIVE_PTR(classname);                             \
                                                                                    \
        byte digest[nativeClassname::DIGESTSIZE];                                   \
        hash->CalculateDigest(digest, (byte*) msg, msgSize);                        \
                                                                                    \
        RETVAL_STRINGL((char*) digest, nativeClassname::DIGESTSIZE, 1);             \
    }

extern zend_class_entry *cryptopp_ce_HashInterface;

void init_interface_HashInterface(TSRMLS_D);

#endif
