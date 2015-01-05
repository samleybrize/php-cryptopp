#include "../php_cryptopp.h"
#include "../exception/php_exception.h"
#include "php_mac_interface.h"
#include <zend_exceptions.h>
#include <exception>
#include <algorithm>
#include "mac_proxy.h"

MacProxy::MacProxy(zval *macObject)
{
    // verify that hashObject is an instance of HashInterface
    if (IS_OBJECT != Z_TYPE_P(macObject) ||
            !instanceof_function(Z_OBJCE_P(macObject), cryptopp_ce_MacInterface)) {
        throw "MacProxy expect a zval that holds an instance of Cryptopp\\MacInterface";
    }

    // retrieve block size once
    zval *funcName;
    zval *zBlockSize;
    MAKE_STD_ZVAL(funcName);
    MAKE_STD_ZVAL(zBlockSize);
    ZVAL_STRING(funcName, "getBlockSize", 1);
    call_user_function(NULL, &macObject, funcName, zBlockSize, 0, NULL TSRMLS_CC);

    if (IS_LONG != Z_TYPE_P(zBlockSize)) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(macObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : invalid block size returned", ce->name);

        zval_dtor(funcName);
        zval_dtor(zBlockSize);
        throw false;
    }

    m_blockSize = static_cast<unsigned int>(Z_LVAL_P(zBlockSize));
    zval_dtor(funcName);
    zval_dtor(zBlockSize);

    // retrieve digest size once
    zval *zDigestSize;
    MAKE_STD_ZVAL(funcName);
    MAKE_STD_ZVAL(zDigestSize);
    ZVAL_STRING(funcName, "getDigestSize", 1);
    call_user_function(NULL, &macObject, funcName, zDigestSize, 0, NULL TSRMLS_CC);

    if (IS_LONG != Z_TYPE_P(zDigestSize)) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(macObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : invalid digest size returned", ce->name);

        zval_dtor(funcName);
        zval_dtor(zDigestSize);
        throw false;
    }

    m_digestSize = static_cast<unsigned int>(Z_LVAL_P(zDigestSize));
    zval_dtor(funcName);
    zval_dtor(zDigestSize);

    // create zvals with php method names
    MAKE_STD_ZVAL(m_funcnameCalculateDigest);
    MAKE_STD_ZVAL(m_funcnameUpdate);
    MAKE_STD_ZVAL(m_funcnameFinal);
    MAKE_STD_ZVAL(m_funcnameRestart);
    MAKE_STD_ZVAL(m_funcnameSetKey);
    ZVAL_STRING(m_funcnameCalculateDigest, "calculateDigest", 1);
    ZVAL_STRING(m_funcnameUpdate, "update", 1);
    ZVAL_STRING(m_funcnameFinal, "finalize", 1);
    ZVAL_STRING(m_funcnameRestart, "restart", 1);
    ZVAL_STRING(m_funcnameSetKey, "setKey", 1);

    // hold macObject
    m_macObject = macObject;
    Z_ADDREF_P(m_macObject);
}

MacProxy::~MacProxy()
{
    Z_DELREF_P(m_macObject);
    zval_dtor(m_funcnameCalculateDigest);
    zval_dtor(m_funcnameUpdate);
    zval_dtor(m_funcnameFinal);
    zval_dtor(m_funcnameRestart);
    zval_dtor(m_funcnameSetKey);
}

unsigned int MacProxy::DigestSize() const {
    return m_digestSize;
}

unsigned int MacProxy::BlockSize() const {
    return m_blockSize;
}

unsigned int MacProxy::OptimalBlockSize() const {
    return m_blockSize;
}

void MacProxy::TruncatedFinal(byte *digest, size_t digestSize) {
    zval *zOutput;
    MAKE_STD_ZVAL(zOutput)
    call_user_function(NULL, &m_macObject, m_funcnameFinal, zOutput, 0, NULL TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(zOutput)) {
        zval_dtor(zOutput);
        throw false;
    } else if (DigestSize() != Z_STRLEN_P(zOutput)) {
        // bad returned digest size
        zend_class_entry *ce;
        ce  = zend_get_class_entry(m_macObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : digest size is %d bytes, returned %d bytes", ce->name, DigestSize(), Z_STRLEN_P(zOutput));

        zval_dtor(zOutput);
        throw false;
    }

    int length = std::min(static_cast<int>(digestSize), Z_STRLEN_P(zOutput));
    memcpy(digest, Z_STRVAL_P(zOutput), length);
    zval_dtor(zOutput);
}

void MacProxy::CalculateDigest(byte *digest, const byte *input, size_t length) {
    zval *zInput;
    zval *zOutput;
    MAKE_STD_ZVAL(zInput)
    MAKE_STD_ZVAL(zOutput)
    ZVAL_STRINGL(zInput, reinterpret_cast<const char*>(input), length, 1);
    call_user_function(NULL, &m_macObject, m_funcnameCalculateDigest, zOutput, 1, &zInput TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(zOutput)) {
        zval_dtor(zInput);
        zval_dtor(zOutput);
        throw false;
    } else if (DigestSize() != Z_STRLEN_P(zOutput)) {
        // bad returned digest size
        zend_class_entry *ce;
        ce  = zend_get_class_entry(m_macObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : digest size is %d bytes, returned %d bytes", ce->name, DigestSize(), Z_STRLEN_P(zOutput));

        zval_dtor(zInput);
        zval_dtor(zOutput);
        throw false;
    }

    memcpy(digest, Z_STRVAL_P(zOutput), Z_STRLEN_P(zOutput));
    zval_dtor(zInput);
    zval_dtor(zOutput);
}

void MacProxy::Update(const byte *input, size_t length) {
    zval *zInput;
    zval *zOutput;
    MAKE_STD_ZVAL(zInput)
    MAKE_STD_ZVAL(zOutput)
    ZVAL_STRINGL(zInput, reinterpret_cast<const char*>(input), length, 1);
    call_user_function(NULL, &m_macObject, m_funcnameUpdate, zOutput, 1, &zInput TSRMLS_CC);

    zval_dtor(zInput);
    zval_dtor(zOutput);
}

void MacProxy::Final(byte *digest) {
    TruncatedFinal(digest, m_digestSize);
}

void MacProxy::Restart() {
    zval *zOutput;
    MAKE_STD_ZVAL(zOutput)
    call_user_function(NULL, &m_macObject, m_funcnameRestart, zOutput, 0, NULL TSRMLS_CC);

    zval_dtor(zOutput);
}

void MacProxy::SetKey(const byte *key, size_t length, const CryptoPP::NameValuePairs &params)
{
    zval *zInput;
    zval *zOutput;
    MAKE_STD_ZVAL(zInput)
    MAKE_STD_ZVAL(zOutput)
    ZVAL_STRINGL(zInput, reinterpret_cast<const char*>(key), length, 1);
    call_user_function(NULL, &m_macObject, m_funcnameSetKey, zOutput, 1, &zInput TSRMLS_CC);

    zval_dtor(zInput);
    zval_dtor(zOutput);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
