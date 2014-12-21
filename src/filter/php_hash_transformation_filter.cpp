#include "../php_cryptopp.h"
#include "../exception/php_exception.h"
#include "../hash/hash_proxy.h"
#include "../hash/php_hash_interface.h"
#include "../hash/php_hash_abstract.h"
#include "../mac/php_mac_interface.h"
#include "../mac/php_mac_abstract.h"
#include "php_hash_transformation_filter.h"
#include <exception>
#include <filters.h>
#include <zend_exceptions.h>

/* {{{ adds hash destruction support to CryptoPP::HashFilter */
HashTransformationFilter::HashTransformationFilter(CryptoPP::HashTransformation &hash, bool hashMustBeDestructed)
    : CryptoPP::HashFilter(hash)
    , m_hash(hash)
    , m_hashMustBeDestructed(hashMustBeDestructed)
{
}

HashTransformationFilter::~HashTransformationFilter()
{
    if (m_hashMustBeDestructed) {
        delete &m_hash;
    }
}
/* }}} */

/* {{{ arg info */
ZEND_BEGIN_ARG_INFO(arginfo_HashTransformationFilter___construct, 0)
    ZEND_ARG_OBJ_INFO(0, hash, Cryptopp\\HashTransformationInterface, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashTransformationFilter___wakeup, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashTransformationFilter___sleep, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashTransformationFilter_getHash, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HashTransformationFilter_calculateDigestString, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ custom object create/free handler */
zend_object_handlers HashTransformationFilter_object_handlers;

void HashTransformationFilter_free_storage(void *object TSRMLS_DC) {
    HashTransformationFilterContainer *obj = static_cast<HashTransformationFilterContainer *>(object);
    delete obj->htf;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}

zend_object_value HashTransformationFilter_create_handler(zend_class_entry *type TSRMLS_DC) {
    zend_object_value retval;

    HashTransformationFilterContainer *obj = static_cast<HashTransformationFilterContainer *>(emalloc(sizeof(HashTransformationFilterContainer)));
    memset(obj, 0, sizeof(HashTransformationFilterContainer));

    zend_object_std_init(&obj->std, type TSRMLS_CC);

    #if PHP_VERSION_ID < 50399
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    #else
        object_properties_init(static_cast<zend_object*>(&(obj->std)), type);
    #endif

    retval.handle   = zend_objects_store_put(obj, NULL, HashTransformationFilter_free_storage, NULL TSRMLS_CC);
    retval.handlers = &HashTransformationFilter_object_handlers;

    return retval;
}
/* }}} */

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_HashTransformationFilter;

static zend_function_entry cryptopp_methods_HashTransformationFilter[] = {
    PHP_ME(Cryptopp_HashTransformationFilter, __construct, arginfo_HashTransformationFilter___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Cryptopp_HashTransformationFilter, __sleep, arginfo_HashTransformationFilter___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_HashTransformationFilter, __wakeup, arginfo_HashTransformationFilter___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_HashTransformationFilter, getHash, arginfo_HashTransformationFilter_getHash, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_HashTransformationFilter, calculateDigestString, arginfo_HashTransformationFilter_calculateDigestString, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_HashTransformationFilter(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "HashTransformationFilter", cryptopp_methods_HashTransformationFilter);
    cryptopp_ce_HashTransformationFilter = zend_register_internal_class(&ce TSRMLS_CC);

    cryptopp_ce_HashTransformationFilter->create_object = HashTransformationFilter_create_handler;
    memcpy(&HashTransformationFilter_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    HashTransformationFilter_object_handlers.clone_obj = NULL;

    zend_declare_property_null(cryptopp_ce_HashTransformationFilter, "hash", 4, ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ get the pointer to the native htf object of the php class */
static HashTransformationFilter *getCryptoppHashTransformationFilterNativePtr(zval *this_ptr TSRMLS_DC) {
    HashTransformationFilter *htf;
    htf = static_cast<HashTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->htf;

    if (NULL == htf) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\HashTransformationFilter : constructor was not called");
    }

    return htf;
}
/* }}} */

/* {{{ set the pointer to the native stf encryptor object of the php class */
static void setCryptoppHashTransformationFilterNativePtr(zval *this_ptr, HashTransformationFilter *nativePtr TSRMLS_DC) {
    static_cast<HashTransformationFilterContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->htf = nativePtr;
}
/* }}} */

/* {{{ indicates if the native hash object holded by a stf object is valid */
static bool isNativeHashTransformationObjectValid(zval *stfObject) {
    zval *hashObject;
    hashObject = zend_read_property(cryptopp_ce_HashTransformationFilter, stfObject, "hash", 4, 0 TSRMLS_CC);

    if (IS_OBJECT != Z_TYPE_P(hashObject)) {
        // not an object
        return false;
    } else if (instanceof_function(Z_OBJCE_P(hashObject), cryptopp_ce_HashAbstract)) {
        // HashAbstract
        return true;
    } else if (instanceof_function(Z_OBJCE_P(hashObject), cryptopp_ce_StreamCipherAbstract)) {
        // MacAbstract
        CryptoPP::MessageAuthenticationCode *mac;
        mac = getCryptoppMacNativePtr(hashObject);

        if (!isCryptoppMacKeyValid(hashObject, mac)) {
            return false;
        }
    }

    return true;
}
/* }}} */

/* {{{ restart the hash holded by a HashTransformationFilter php object */
static void restartHashObject(zval *htfObject) {
    zval *hashObject;
    hashObject = zend_read_property(cryptopp_ce_HashTransformationFilter, htfObject, "hash", 4, 0 TSRMLS_CC);

    zval *funcName;
    MAKE_STD_ZVAL(funcName)
    ZVAL_STRING(funcName, "restart", 1);
    call_user_function(NULL, &hashObject, funcName, funcName, 0, NULL TSRMLS_CC);
    zval_dtor(funcName);
}
/* }}} */

/* {{{ returns the hash object digest size */
static int getHashObjectDigestSize(zval *htfObject) {
    zval *hashObject;
    hashObject = zend_read_property(cryptopp_ce_HashTransformationFilter, htfObject, "hash", 4, 0 TSRMLS_CC);

    zval *funcName;
    zval *zDigestSize;
    MAKE_STD_ZVAL(funcName)
    MAKE_STD_ZVAL(zDigestSize)
    ZVAL_STRING(funcName, "getDigestSize", 1);
    call_user_function(NULL, &hashObject, funcName, zDigestSize, 0, NULL TSRMLS_CC);

    if (IS_LONG != Z_TYPE_P(zDigestSize)) {
        throw false;
    }

    int digestSize = static_cast<int>(Z_LVAL_P(zDigestSize));
    zval_dtor(funcName);
    zval_dtor(zDigestSize);

    return digestSize;
}
/* }}} */

/* {{{ proto void HashTransformationFilter::__sleep(void)
   Prevents serialization of a HashTransformationFilter instance */
PHP_METHOD(Cryptopp_HashTransformationFilter, __sleep) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\HashTransformationFilter instances");
}
/* }}} */

/* {{{ proto void HashTransformationFilter::__wakeup(void)
   Prevents use of a HashTransformationFilter instance that has been unserialized */
PHP_METHOD(Cryptopp_HashTransformationFilter, __wakeup) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\HashTransformationFilter instances");
}
/* }}} */

/* {{{ proto HashTransformationFilter::__construct(Cryptopp\HashTransformationInterface hash) */
PHP_METHOD(Cryptopp_HashTransformationFilter, __construct) {
    zval *hashObject;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O|O", &hashObject, cryptopp_ce_HashTransformationInterface)) {
        return;
    }

    // retrieve native hash object
    CryptoPP::HashTransformation *hash;
    bool parentConstructorError = false;
    bool hashMustBeDestructed   = false;

    if (instanceof_function(Z_OBJCE_P(hashObject), cryptopp_ce_HashAbstract)) {
        // retrieve native hash object
        hash = getCryptoppHashNativePtr(hashObject);

        if (NULL == hash) {
            parentConstructorError = true;
        }
    } else if (instanceof_function(Z_OBJCE_P(hashObject), cryptopp_ce_MacAbstract)) {
        // retrieve native mac object
        hash = getCryptoppMacNativePtr(hashObject);

        if (NULL == hash) {
            parentConstructorError = true;
        }
    } else {
        // create a proxy to the user php object
        try {
            hash                    = new HashProxy(hashObject);
            hashMustBeDestructed    = true;
        } catch (bool e) {
            return;
        } catch (const char *e) {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp internal error: HashTransformationFilter: %s", e);
            return;
        }
    }

    if (parentConstructorError) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(hashObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : parent constructor was not called", ce->name);
        return;
    }

    // create native symmetric transformation filter
    HashTransformationFilter *htf;

    try {
        htf = new HashTransformationFilter(*hash, hashMustBeDestructed);
    } catch (std::exception &e) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp internal error: HashTransformationFilter: %s", e.what());
        return;
    }

    setCryptoppHashTransformationFilterNativePtr(getThis(), htf TSRMLS_CC);

    // hold the hash object. if not, it can be deleted.
    // same for padding
    zend_update_property(cryptopp_ce_HashTransformationFilter, getThis(), "hash", 4, hashObject TSRMLS_CC);
}
/* }}} */

/* {{{ proto Cryptopp\HashTransformationInterface HashTransformationFilter::getHash(void)
       Returns the hash object */
PHP_METHOD(Cryptopp_HashTransformationFilter, getHash) {
    zval *hashObject;
    hashObject = zend_read_property(cryptopp_ce_HashTransformationFilter, getThis(), "hash", 4, 0 TSRMLS_CC);
    RETURN_ZVAL(hashObject, 1, 0)
}
/* }}} */

// TODO method encrypt: accept an input and output object (return boolean)

/* {{{ proto bool|string HashTransformationFilter::calculateDigestString(string data)
       Calculate the digest of a string */
PHP_METHOD(Cryptopp_HashTransformationFilter, calculateDigestString) {
    char *data      = NULL;
    int dataSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &dataSize)) {
        return;
    }

    HashTransformationFilter *htf;
    htf = CRYPTOPP_HASH_TRANSFORMATION_FILTER_GET_NATIVE_PTR(htf)

    // if the hash object is a native mac object, ensure that the key is valid
    if (!isNativeHashTransformationObjectValid(getThis())) {
        RETURN_FALSE
    }

    // calculate digest
    try {
        htf->GetNextMessage();
        size_t written  = htf->Put(reinterpret_cast<byte*>(data), dataSize);
        htf->MessageEnd();

        CryptoPP::lword retrievable = htf->MaxRetrievable();
        restartHashObject(getThis());

        if (retrievable == getHashObjectDigestSize(getThis())) {
            // return ciphertext
            byte digest[retrievable];
            htf->Get(digest, retrievable);
            RETURN_STRINGL(reinterpret_cast<char*>(digest), retrievable, 1)
        } else {
            // something goes wrong
            RETURN_FALSE
        }
    } catch (bool e) {
        RETURN_FALSE
    }
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
