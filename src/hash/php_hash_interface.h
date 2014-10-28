#ifndef HASH_HASH_INTERFACE_H
#define HASH_HASH_INTERFACE_H

#include "../php_cryptopp.h"

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
                                                                            \
        if (PHP_VERSION_ID < 50399) {                                       \
            zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *)); \
        } else {                                                            \
            object_properties_init((zend_object*) &(obj->std), type);       \
        }                                                                   \
                                                                            \
        retval.handle   = zend_objects_store_put(obj, NULL, classname ## _free_storage, NULL TSRMLS_CC); \
        retval.handlers = &classname ## _object_handlers;                   \
                                                                            \
        return retval;                                                      \
    }                                                                       \
    CRYPTOPP_HASH_INIT_CLASS_FUNC_HEADER(classname) {                       \
        zend_class_entry ce;                                                \
        INIT_NS_CLASS_ENTRY(ce, PHP_CRYPTOPP_NAMESPACE, #classname, classMethodsVarName); \
        classEntryPtrName = zend_register_internal_class(&ce TSRMLS_CC);    \
                                                                            \
        classEntryPtrName->create_object = classname ## _create_handler;    \
        memcpy(&classname ## _object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
        classname ## _object_handlers.clone_obj = NULL;                     \
                                                                            \
        zend_class_implements(classEntryPtrName TSRMLS_CC, 1, cryptopp_ce_HashInterface); \
    }

#define CRYPTOPP_HASH_INIT_CLASS_FUNC_HEADER(classname) void init_class_ ## classname(TSRMLS_D)
#define CRYPTOPP_HASH_INIT_CLASS_FUNC_CALL(classname) init_class_ ## classname(TSRMLS_C);

#define CRYPTOPP_HASH_GET_NATIVE_PTR(classname) ((classname ## Container *)zend_object_store_get_object(getThis() TSRMLS_CC))->hash
#define CRYPTOPP_HASH_SET_NATIVE_PTR(classname, nativeHashPtr) ((classname ## Container *)zend_object_store_get_object(getThis() TSRMLS_CC))->hash = nativeHashPtr;

#define CRYPTOPP_HASH_REQUIRED_METHODS(classname) \
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_ ## classname, calculateDigest, arginfo_HashInterface_calculateDigest, ZEND_ACC_PUBLIC)

#define CRYPTOPP_HASH_REQUIRED_METHODS_HEADER(classname) \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, calculateDigest);

#define CRYPTOPP_HASH_REQUIRED_METHODS_DEFINITIONS(classname, nativeClassname)      \
    PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_ ## classname, calculateDigest) {         \
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

ZEND_BEGIN_ARG_INFO(arginfo_HashInterface_calculateDigest, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

extern zend_class_entry *cryptopp_ce_HashInterface;

void init_interface_HashInterface(TSRMLS_D);

#endif
