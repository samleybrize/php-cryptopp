/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_ZEND_OBJECT_UTILS_H
#define PHP_ZEND_OBJECT_UTILS_H

#include "src/php_cryptopp.h"

bool isCryptoppSymmetricKeyValid(zval *object, CryptoPP::SimpleKeyingInterface *keying, int keySize TSRMLS_DC, bool throwIfFalse = true);
bool isCryptoppSymmetricIvValid(zval *object, CryptoPP::SimpleKeyingInterface *keying, int ivSize TSRMLS_DC, bool throwIfFalse = true);

bool setSymmetricCipherKeyIv(
    zval *object,
    CryptoPP::SimpleKeyingInterface *encryptor,
    CryptoPP::SimpleKeyingInterface *decryptor,
    zval *zKey,
    zval *zIv
    TSRMLS_DC
);

typedef void(*zend_object_free_storage)(void* TSRMLS_DC);

template<class CONTAINER, zend_object_free_storage FREE_STORAGE, zend_object_handlers *OBJECT_HANDLERS>
zend_object_value zend_custom_create_handler(zend_class_entry *type TSRMLS_DC) {
    zend_object_value retval;

    CONTAINER *obj = static_cast<CONTAINER *>(emalloc(sizeof(CONTAINER)));
    memset(obj, 0, sizeof(CONTAINER));

    zend_object_std_init(&obj->std, type TSRMLS_CC);
    object_properties_init(&obj->std, type);
//    ALLOC_HASHTABLE(obj->std.properties);
//    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);

//    #if PHP_VERSION_ID < 50399
//        zval *tmp;
//        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
//    #else
//        object_properties_init(static_cast<zend_object*>(&(obj->std)), type);
//    #endif

    retval.handle   = zend_objects_store_put(obj, (zend_objects_store_dtor_t) zend_objects_destroy_object, FREE_STORAGE, NULL TSRMLS_CC);
    retval.handlers = OBJECT_HANDLERS;

    return retval;
}

#endif /* PHP_ZEND_OBJECT_UTILS_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
