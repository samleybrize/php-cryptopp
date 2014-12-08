#include "../php_cryptopp.h"
#include "../exception/php_exception.h"
#include "php_rbg_interface.h"
#include "php_rbg.h"
#include <osrng.h>
#include <zend_exceptions.h>

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_RandomByteGenerator_construct, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ custom object create/free handler */
zend_object_handlers RandomByteGenerator_object_handlers;

void RandomByteGenerator_free_storage(void *object TSRMLS_DC) {
    RandomByteGeneratorContainer *obj = static_cast<RandomByteGeneratorContainer *>(object);
    delete obj->rbg;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}

zend_object_value RandomByteGenerator_create_handler(zend_class_entry *type TSRMLS_DC) {
    zend_object_value retval;

    RandomByteGeneratorContainer *obj = static_cast<RandomByteGeneratorContainer *>(emalloc(sizeof(RandomByteGeneratorContainer)));
    memset(obj, 0, sizeof(RandomByteGeneratorContainer));

    zend_object_std_init(&obj->std, type TSRMLS_CC);

    #if PHP_VERSION_ID < 50399
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    #else
        object_properties_init(static_cast<zend_object*>(&(obj->std)), type);
    #endif

    retval.handle   = zend_objects_store_put(obj, NULL, RandomByteGenerator_free_storage, NULL TSRMLS_CC);
    retval.handlers = &RandomByteGenerator_object_handlers;

    return retval;
}
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_RandomByteGenerator;

static zend_function_entry cryptopp_methods_RandomByteGenerator[] = {
    PHP_ME(Cryptopp_RandomByteGenerator, __construct, arginfo_RandomByteGenerator_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Cryptopp_RandomByteGenerator, __sleep, arginfo_RandomByteGeneratorInterface___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_RandomByteGenerator, __wakeup, arginfo_RandomByteGeneratorInterface___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_RandomByteGenerator, generate, arginfo_RandomByteGeneratorInterface_generate, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_RandomByteGenerator(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "RandomByteGenerator", cryptopp_methods_RandomByteGenerator);
    cryptopp_ce_RandomByteGenerator = zend_register_internal_class(&ce TSRMLS_CC);

    cryptopp_ce_RandomByteGenerator->create_object = RandomByteGenerator_create_handler;
    memcpy(&RandomByteGenerator_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    RandomByteGenerator_object_handlers.clone_obj = NULL;

    zend_class_implements(cryptopp_ce_RandomByteGenerator TSRMLS_CC, 1, cryptopp_ce_RandomByteGeneratorInterface);
}
/* }}} */

/* {{{ proto RandomByteGenerator::__construct(void) */
PHP_METHOD(Cryptopp_RandomByteGenerator, __construct) {
    CryptoPP::AutoSeededRandomPool *rbg;
    rbg = new CryptoPP::AutoSeededRandomPool();

    static_cast<RandomByteGeneratorContainer *>(zend_object_store_get_object(getThis() TSRMLS_CC))->rbg = rbg;
}
/* }}} */

/* {{{ proto string RandomByteGenerator::__sleep(void)
   Prevents serialization of a RandomByteGenerator instance */
PHP_METHOD(Cryptopp_RandomByteGenerator, __sleep) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\RandomByteGenerator instances");
}
/* }}} */

/* {{{ proto string RandomByteGenerator::__wakeup(void)
   Prevents use of a RandomByteGenerator instance that has been unserialized */
PHP_METHOD(Cryptopp_RandomByteGenerator, __wakeup) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\RandomByteGenerator instances");
}
/* }}} */

/* {{{ proto string RandomByteGenerator::generate(int size)
   Generates random byte sequence */
PHP_METHOD(Cryptopp_RandomByteGenerator, generate) {
    long size = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &size)) {
        return;
    }

    // verify that size > 0
    if (size <= 0) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Size must be a positive integer, %ld given", size);
        RETURN_FALSE;
    }

    // generate random bytes
    CryptoPP::RandomNumberGenerator *rbg;
    rbg = static_cast<RandomByteGeneratorContainer *>(zend_object_store_get_object(getThis() TSRMLS_CC))->rbg;

    if (NULL == rbg) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Constructor was not called");
        RETURN_FALSE;
    }

    byte block[size];
    rbg->GenerateBlock(block, size);

    RETVAL_STRINGL(reinterpret_cast<char*>(block), size, 1);
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
