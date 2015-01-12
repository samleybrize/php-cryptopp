#include "../php_cryptopp.h"
#include "../exception/php_exception.h"
#include "../utils/zval_utils.h"
#include "hash_proxy.h"
#include "php_hash_transformation_interface.h"
#include <zend_exceptions.h>
#include <exception>
#include <algorithm>

HashProxy::HashProxy(zval *hashObject TSRMLS_DC)
{
    M_TSRMLS_C = TSRMLS_C;

    // verify that hashObject is an instance of HashInterface
    if (IS_OBJECT != Z_TYPE_P(hashObject) ||
            !instanceof_function(Z_OBJCE_P(hashObject), cryptopp_ce_HashTransformationInterface TSRMLS_CC)) {
        throw "HashProxy expect a zval that holds an instance of Cryptopp\\HashTransformationInterface";
    }

    // retrieve block size once
    zval *funcName      = makeZval("getBlockSize");
    zval *zBlockSize    = call_user_method(hashObject, funcName TSRMLS_CC);

    if (IS_LONG != Z_TYPE_P(zBlockSize)) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(hashObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : invalid block size returned", ce->name);

        zval_ptr_dtor(&funcName);
        zval_ptr_dtor(&zBlockSize);
        throw false;
    }

    m_blockSize = static_cast<unsigned int>(Z_LVAL_P(zBlockSize));
    zval_ptr_dtor(&funcName);
    zval_ptr_dtor(&zBlockSize);

    // retrieve digest size once
    funcName            = makeZval("getDigestSize");
    zval *zDigestSize   = call_user_method(hashObject, funcName TSRMLS_CC);

    if (IS_LONG != Z_TYPE_P(zDigestSize)) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(hashObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : invalid digest size returned", ce->name);

        zval_ptr_dtor(&funcName);
        zval_ptr_dtor(&zDigestSize);
        throw false;
    }

    m_digestSize = static_cast<unsigned int>(Z_LVAL_P(zDigestSize));
    zval_ptr_dtor(&funcName);
    zval_ptr_dtor(&zDigestSize);

    // create zvals with php method names
    m_funcnameCalculateDigest   = makeZval("calculateDigest");
    m_funcnameUpdate            = makeZval("update");
    m_funcnameFinal             = makeZval("finalize");
    m_funcnameRestart           = makeZval("restart");

    // hold hashObject
    m_hashObject = hashObject;
    Z_ADDREF_P(m_hashObject);
}

HashProxy::~HashProxy()
{
    zval_ptr_dtor(&m_hashObject);
    zval_ptr_dtor(&m_funcnameCalculateDigest);
    zval_ptr_dtor(&m_funcnameUpdate);
    zval_ptr_dtor(&m_funcnameFinal);
    zval_ptr_dtor(&m_funcnameRestart);
}

unsigned int HashProxy::DigestSize() const {
    return m_digestSize;
}

unsigned int HashProxy::BlockSize() const {
    return m_blockSize;
}

unsigned int HashProxy::OptimalBlockSize() const {
    return m_blockSize;
}

void HashProxy::TruncatedFinal(byte *digest, size_t digestSize) {
    zval *zOutput = call_user_method(m_hashObject, m_funcnameFinal M_TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(zOutput)) {
        zval_ptr_dtor(&zOutput);
        throw false;
    } else if (DigestSize() != Z_STRLEN_P(zOutput)) {
        // bad returned digest size
        zend_class_entry *ce;
        ce  = zend_get_class_entry(m_hashObject M_TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 M_TSRMLS_CC, (char*)"%s : digest size is %d bytes, returned %d bytes", ce->name, DigestSize(), Z_STRLEN_P(zOutput));

        zval_ptr_dtor(&zOutput);
        throw false;
    }

    int length = std::min(static_cast<int>(digestSize), Z_STRLEN_P(zOutput));
    memcpy(digest, Z_STRVAL_P(zOutput), length);
    zval_ptr_dtor(&zOutput);
}

void HashProxy::CalculateDigest(byte *digest, const byte *input, size_t length) {
    zval *zInput    = makeZval(reinterpret_cast<const char*>(input), length);
    zval *zOutput   = call_user_method(m_hashObject, m_funcnameCalculateDigest, zInput M_TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(zOutput)) {
        zval_ptr_dtor(&zInput);
        zval_ptr_dtor(&zOutput);
        throw false;
    } else if (DigestSize() != Z_STRLEN_P(zOutput)) {
        // bad returned digest size
        zend_class_entry *ce;
        ce  = zend_get_class_entry(m_hashObject M_TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 M_TSRMLS_CC, (char*)"%s : digest size is %d bytes, returned %d bytes", ce->name, DigestSize(), Z_STRLEN_P(zOutput));

        zval_ptr_dtor(&zInput);
        zval_ptr_dtor(&zOutput);
        throw false;
    }

    memcpy(digest, Z_STRVAL_P(zOutput), Z_STRLEN_P(zOutput));
    zval_ptr_dtor(&zInput);
    zval_ptr_dtor(&zOutput);
}

void HashProxy::Update(const byte *input, size_t length) {
    zval *zInput    = makeZval(reinterpret_cast<const char*>(input), length);
    zval *zOutput   = call_user_method(m_hashObject, m_funcnameUpdate, zInput M_TSRMLS_CC);

    zval_ptr_dtor(&zInput);
    zval_ptr_dtor(&zOutput);
}

void HashProxy::Final(byte *digest) {
    TruncatedFinal(digest, m_digestSize);
}

void HashProxy::Restart() {
    zval *zOutput = call_user_method(m_hashObject, m_funcnameRestart M_TSRMLS_CC);
    zval_ptr_dtor(&zOutput);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
