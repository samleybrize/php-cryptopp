/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "src/php_cryptopp.h"
#include "src/exception/php_exception.h"
#include "src/utils/zval_utils.h"
#include "authenticated_symmetric_cipher_proxy.h"
#include "php_authenticated_symmetric_cipher_interface.h"
#include <zend_exceptions.h>

AuthenticatedSymmetricCipherProxy::Base::Base(zval *authenticatedCipherObject, const char* processDataFuncname, const char* updateFuncname, const char* finalizeFuncname TSRMLS_DC)
{
    M_TSRMLS_C = TSRMLS_C;

    // verify that authenticatedCipherObject is an instance of AuthenticatedSymmetricCipherInterface
    if (IS_OBJECT != Z_TYPE_P(authenticatedCipherObject) ||
            !instanceof_function(Z_OBJCE_P(authenticatedCipherObject), cryptopp_ce_AuthenticatedSymmetricCipherInterface TSRMLS_CC)) {
        throw "AuthenticatedSymmetricCipherProxy expect a zval that holds an instance of Cryptopp\\AuthenticatedSymmetricCipherInterface";
    }

    // retrieve block size once
    zval *funcName      = makeZval("getBlockSize");
    zval *zBlockSize    = call_user_method(authenticatedCipherObject, funcName TSRMLS_CC);

    if (IS_LONG != Z_TYPE_P(zBlockSize)) {
        zend_class_entry *ce = zend_get_class_entry(authenticatedCipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : invalid block size returned", ce->name);

        zval_ptr_dtor(&funcName);
        zval_ptr_dtor(&zBlockSize);
        throw false;
    }

    m_blockSize = static_cast<unsigned int>(Z_LVAL_P(zBlockSize));
    zval_ptr_dtor(&zBlockSize);
    zval_ptr_dtor(&funcName);

    // retrieve digest size once
    funcName            = makeZval("getDigestSize");
    zval *zDigestSize   = call_user_method(authenticatedCipherObject, funcName TSRMLS_CC);

    if (IS_LONG != Z_TYPE_P(zDigestSize)) {
        zend_class_entry *ce = zend_get_class_entry(authenticatedCipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : invalid block size returned", ce->name);

        zval_ptr_dtor(&funcName);
        zval_ptr_dtor(&zDigestSize);
        throw false;
    }

    m_digestSize = static_cast<unsigned int>(Z_LVAL_P(zDigestSize));
    zval_ptr_dtor(&zDigestSize);
    zval_ptr_dtor(&funcName);

    // retrieve algo name once
    funcName    = makeZval("getName");
    zval *zName = call_user_method(authenticatedCipherObject, funcName TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(zName)) {
        ZVAL_STRING(zName, "User", 1);
    }

    m_name.assign(Z_STRVAL_P(zName), Z_STRLEN_P(zName));
    zval_ptr_dtor(&funcName);
    zval_ptr_dtor(&zName);

    // create zvals with php method names
    m_funcnameProcessData   = makeZval(processDataFuncname);
    m_funcnameUpdate        = makeZval(updateFuncname);
    m_funcnameFinalize      = makeZval(finalizeFuncname);
    m_funcnameRestart       = makeZval("restart");

    // hold authenticatedCipherObject
    m_authenticatedCipherObject = authenticatedCipherObject;
    Z_ADDREF_P(m_authenticatedCipherObject);
}

AuthenticatedSymmetricCipherProxy::Base::~Base()
{
    zval_ptr_dtor(&m_authenticatedCipherObject);
    zval_ptr_dtor(&m_funcnameProcessData);
    zval_ptr_dtor(&m_funcnameUpdate);
    zval_ptr_dtor(&m_funcnameFinalize);
    zval_ptr_dtor(&m_funcnameRestart);
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
    zval *input     = makeZval(reinterpret_cast<const char*>(inString), length);
    zval *output    = call_user_method(m_authenticatedCipherObject, m_funcnameProcessData, input M_TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(output)) {
        zval_ptr_dtor(&input);
        zval_ptr_dtor(&output);
        throw false;
    }

    memcpy(outString, Z_STRVAL_P(output), Z_STRLEN_P(output));
    zval_ptr_dtor(&input);
    zval_ptr_dtor(&output);
}

void AuthenticatedSymmetricCipherProxy::Base::Update(const byte *input, size_t length)
{
    zval *zInput    = makeZval(reinterpret_cast<const char*>(input), length);
    zval *zOutput   = call_user_method(m_authenticatedCipherObject, m_funcnameUpdate, zInput M_TSRMLS_CC);

    zval_ptr_dtor(&zInput);
    zval_ptr_dtor(&zOutput);
}

void AuthenticatedSymmetricCipherProxy::Base::TruncatedFinal(byte *digest, size_t digestSize)
{
    zval *zOutput = call_user_method(m_authenticatedCipherObject, m_funcnameFinalize M_TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(zOutput)) {
        zval_ptr_dtor(&zOutput);
        throw false;
    } else if (DigestSize() != Z_STRLEN_P(zOutput)) {
        // bad returned digest size
        zend_class_entry *ce = zend_get_class_entry(m_authenticatedCipherObject M_TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 M_TSRMLS_CC, (char*)"%s : digest size is %d bytes, returned %d bytes", ce->name, DigestSize(), Z_STRLEN_P(zOutput));

        zval_ptr_dtor(&zOutput);
        throw false;
    }

    int length = std::min(static_cast<int>(digestSize), Z_STRLEN_P(zOutput));
    memcpy(digest, Z_STRVAL_P(zOutput), length);
    zval_ptr_dtor(&zOutput);
}

void AuthenticatedSymmetricCipherProxy::Base::Resynchronize(const byte *iv, int ivLength)
{
    Restart();
}

void AuthenticatedSymmetricCipherProxy::Base::Restart()
{
    zval *zOutput = call_user_method(m_authenticatedCipherObject, m_funcnameRestart M_TSRMLS_CC);
    zval_ptr_dtor(&zOutput);
}
