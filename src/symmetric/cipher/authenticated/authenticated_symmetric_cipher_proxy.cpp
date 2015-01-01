#include "../../../php_cryptopp.h"
#include "../../../exception/php_exception.h"
#include "authenticated_symmetric_cipher_proxy.h"
#include "php_authenticated_symmetric_cipher_interface.h"
#include <zend_exceptions.h>

AuthenticatedSymmetricCipherProxy::Base::Base(zval *authenticatedCipherObject, const char* processDataFuncname, const char* updateFuncname, const char* finalizeFuncname)
{
    // verify that authenticatedCipherObject is an instance of AuthenticatedSymmetricCipherInterface
    if (IS_OBJECT != Z_TYPE_P(authenticatedCipherObject) ||
            !instanceof_function(Z_OBJCE_P(authenticatedCipherObject), cryptopp_ce_AuthenticatedSymmetricCipherInterface)) {
        throw "AuthenticatedSymmetricCipherProxy expect a zval that holds an instance of Cryptopp\\AuthenticatedSymmetricCipherInterface";
    }

    // retrieve block size once
    zval *funcName;
    zval *zBlockSize;
    MAKE_STD_ZVAL(funcName);
    MAKE_STD_ZVAL(zBlockSize);
    ZVAL_STRING(funcName, "getBlockSize", 1);
    call_user_function(NULL, &authenticatedCipherObject, funcName, zBlockSize, 0, NULL TSRMLS_CC);

    if (IS_LONG != Z_TYPE_P(zBlockSize)) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(authenticatedCipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : invalid block size returned", ce->name);

        zval_dtor(funcName);
        zval_dtor(zBlockSize);
        throw false;
    }

    m_blockSize = static_cast<unsigned int>(Z_LVAL_P(zBlockSize));
    zval_dtor(zBlockSize);

    // retrieve digest size once
    zval *zDigestSize;
    MAKE_STD_ZVAL(zDigestSize);
    ZVAL_STRING(funcName, "getDigestSize", 1);
    call_user_function(NULL, &authenticatedCipherObject, funcName, zDigestSize, 0, NULL TSRMLS_CC);

    if (IS_LONG != Z_TYPE_P(zDigestSize)) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(authenticatedCipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : invalid block size returned", ce->name);

        zval_dtor(funcName);
        zval_dtor(zDigestSize);
        throw false;
    }

    m_digestSize = static_cast<unsigned int>(Z_LVAL_P(zDigestSize));
    zval_dtor(zDigestSize);

    // retrieve algo name once
    zval *zName;
    MAKE_STD_ZVAL(zName);
    ZVAL_STRING(funcName, "getName", 1);
    call_user_function(NULL, &authenticatedCipherObject, funcName, zName, 0, NULL TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(zName)) {
        ZVAL_STRING(zName, "User", 1);
    }

    m_name.assign(Z_STRVAL_P(zName), Z_STRLEN_P(zName));
    zval_dtor(funcName);
    zval_dtor(zName);

    // create a zval with the php method names to call
    MAKE_STD_ZVAL(m_funcnameProcessData);
    MAKE_STD_ZVAL(m_funcnameUpdate);
    MAKE_STD_ZVAL(m_funcnameFinalize);
    MAKE_STD_ZVAL(m_funcnameRestart);
    ZVAL_STRING(m_funcnameProcessData, processDataFuncname, 1);
    ZVAL_STRING(m_funcnameUpdate, updateFuncname, 1);
    ZVAL_STRING(m_funcnameFinalize, finalizeFuncname, 1);
    ZVAL_STRING(m_funcnameRestart, "restart", 1);

    // hold authenticatedCipherObject
    m_authenticatedCipherObject = authenticatedCipherObject;
    Z_ADDREF_P(m_authenticatedCipherObject);
}

AuthenticatedSymmetricCipherProxy::Base::~Base()
{
    Z_DELREF_P(m_authenticatedCipherObject);
    zval_dtor(m_funcnameProcessData);
    zval_dtor(m_funcnameUpdate);
    zval_dtor(m_funcnameFinalize);
    zval_dtor(m_funcnameRestart);
}

unsigned int AuthenticatedSymmetricCipherProxy::Base::MandatoryBlockSize() const
{
    return m_blockSize;
}

unsigned int AuthenticatedSymmetricCipherProxy::Base::OptimalBlockSize() const
{
    return m_blockSize;
}

unsigned int AuthenticatedSymmetricCipherProxy::Base::DigestSize() const
{
    return m_digestSize;
}

unsigned int AuthenticatedSymmetricCipherProxy::Base::MinLastBlockSize() const
{
    return 0;
}

void AuthenticatedSymmetricCipherProxy::Base::ProcessData(byte *outString, const byte *inString, size_t length)
{
    zval *input;
    zval *output;
    MAKE_STD_ZVAL(input)
    MAKE_STD_ZVAL(output)
    ZVAL_STRINGL(input, reinterpret_cast<const char*>(inString), length, 1);
    call_user_function(NULL, &m_authenticatedCipherObject, m_funcnameProcessData, output, 1, &input TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(output)) {
        zval_dtor(input);
        zval_dtor(output);
        throw false;
    }

    memcpy(outString, Z_STRVAL_P(output), Z_STRLEN_P(output));
    zval_dtor(input);
    zval_dtor(output);
}

void AuthenticatedSymmetricCipherProxy::Base::Update(const byte *input, size_t length)
{
    zval *zInput;
    zval *zOutput;
    MAKE_STD_ZVAL(zInput)
    MAKE_STD_ZVAL(zOutput)
    ZVAL_STRINGL(zInput, reinterpret_cast<const char*>(input), length, 1);
    call_user_function(NULL, &m_authenticatedCipherObject, m_funcnameUpdate, zOutput, 1, &zInput TSRMLS_CC);

    zval_dtor(zInput);
    zval_dtor(zOutput);
}

void AuthenticatedSymmetricCipherProxy::Base::TruncatedFinal(byte *digest, size_t digestSize)
{
    zval *zOutput;
    MAKE_STD_ZVAL(zOutput)
    call_user_function(NULL, &m_authenticatedCipherObject, m_funcnameFinalize, zOutput, 0, NULL TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(zOutput)) {
        zval_dtor(zOutput);
        throw false;
    } else if (DigestSize() != Z_STRLEN_P(zOutput)) {
        // bad returned digest size
        zend_class_entry *ce;
        ce  = zend_get_class_entry(m_authenticatedCipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : digest size is %d bytes, returned %d bytes", ce->name, DigestSize(), Z_STRLEN_P(zOutput));

        zval_dtor(zOutput);
        throw false;
    }

    int length = std::min(static_cast<int>(digestSize), Z_STRLEN_P(zOutput));
    memcpy(digest, Z_STRVAL_P(zOutput), length);
    zval_dtor(zOutput);
}

void AuthenticatedSymmetricCipherProxy::Base::Restart()
{
    zval *zOutput;
    MAKE_STD_ZVAL(zOutput)
    call_user_function(NULL, &m_authenticatedCipherObject, m_funcnameRestart, zOutput, 0, NULL TSRMLS_CC);
    zval_dtor(zOutput);
}
