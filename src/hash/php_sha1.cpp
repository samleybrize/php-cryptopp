#include "../php_cryptopp_test.h"
#include "php_hash_interface.h"
#include "php_sha1.h"
#include <sha.h>
#include <map>

zend_object_handlers HashSha1_object_handlers;

struct HashSha1Container {
    zend_object std;
    CryptoPP::SHA1 *hash;
};

void HashSha1_free_storage(void *object TSRMLS_DC)
{
    HashSha1Container *obj = (HashSha1Container *) object;
    delete obj->hash;

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);
}

zend_object_value HashSha1_create_handler(zend_class_entry *type TSRMLS_DC)
{
    zval *tmp;
    zend_object_value retval;

    HashSha1Container *obj = (HashSha1Container *) emalloc(sizeof(HashSha1Container));
    memset(obj, 0, sizeof(HashSha1Container));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    // TODO type->properties_info ?????

    retval.handle   = zend_objects_store_put(obj, NULL, HashSha1_free_storage, NULL TSRMLS_CC);
    retval.handlers = &HashSha1_object_handlers;

    return retval;
}

/*
 * PHP class déclaration
 */
zend_class_entry *cryptopp_test_ce_hash_sha1;

static zend_function_entry hash_sha1_methods[] = {
    PHP_ME(HashSha1, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(HashSha1, hash, NULL, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void init_class_HashSha1(TSRMLS_D) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "HashSha1", hash_sha1_methods);
    cryptopp_test_ce_hash_sha1                  = zend_register_internal_class(&ce TSRMLS_CC);

    cryptopp_test_ce_hash_sha1->create_object   = HashSha1_create_handler;
    memcpy(&HashSha1_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    HashSha1_object_handlers.clone_obj          = NULL;

    zend_class_implements(cryptopp_test_ce_hash_sha1 TSRMLS_CC, 1, cryptopp_test_ce_hash_interface);
}

/*
 * PHP methods definitions
 */
PHP_METHOD(HashSha1, __construct) {
    CryptoPP::SHA1 *hash;
    hash                    = new CryptoPP::SHA1();
    HashSha1Container *obj  = (HashSha1Container *)zend_object_store_get_object(getThis() TSRMLS_CC);
    obj->hash               = hash;
}

PHP_METHOD(HashSha1, hash) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::SHA1 *hash;
    HashSha1Container *obj  = (HashSha1Container *)zend_object_store_get_object(getThis() TSRMLS_CC);
    hash                    = obj->hash;

    byte digest[CryptoPP::SHA1::DIGESTSIZE];
    hash->CalculateDigest(digest, (byte*) msg, msgSize);

    RETVAL_STRINGL((char*) digest, CryptoPP::SHA1::DIGESTSIZE, 1);
}
