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
#include "mac_proxy.h"
#include "php_mac_interface.h"
#include <zend_exceptions.h>
#include <exception>
#include <algorithm>

MacProxy::MacProxy(zval *macObject TSRMLS_DC)
{
    SET_M_TSRMLS_C()

    // verify that hashObject is an instance of HashInterface
    if (IS_OBJECT != Z_TYPE_P(macObject) ||
            !instanceof_function(Z_OBJCE_P(macObject), cryptopp_ce_MacInterface TSRMLS_CC)) {
        throw "MacProxy expect a zval that holds an instance of Cryptopp\\MacInterface";
    }

    // retrieve block size once
    zval *funcName      = makeZval("getBlockSize");
    zval *zBlockSize    = call_user_method(macObject, funcName TSRMLS_CC);

    if (IS_LONG != Z_TYPE_P(zBlockSize)) {
        zend_class_entry *ce = zend_get_class_entry(macObject TSRMLS_CC);
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
    zval *zDigestSize   = call_user_method(macObject, funcName TSRMLS_CC);

    if (IS_LONG != Z_TYPE_P(zDigestSize)) {
        zend_class_entry *ce = zend_get_class_entry(macObject TSRMLS_CC);
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
    m_funcnameSetKey            = makeZval("setKey");
    m_funcnameIsValidKeyLength  = makeZval("isValidKeyLength");

    // hold macObject
    m_macObject = macObject;
    Z_ADDREF_P(m_macObject);
}

MacProxy::~MacProxy()
{
    zval_ptr_dtor(&m_macObject);
    zval_ptr_dtor(&m_funcnameCalculateDigest);
    zval_ptr_dtor(&m_funcnameUpdate);
    zval_ptr_dtor(&m_funcnameFinal);
    zval_ptr_dtor(&m_funcnameRestart);
    zval_ptr_dtor(&m_funcnameSetKey);
    zval_ptr_dtor(&m_funcnameIsValidKeyLength);
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
    zval *zOutput = call_user_method(m_macObject, m_funcnameFinal M_TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(zOutput)) {
        zval_ptr_dtor(&zOutput);
        throw false;
    } else if (DigestSize() != Z_STRLEN_P(zOutput)) {
        // bad returned digest size
        zend_class_entry *ce = zend_get_class_entry(m_macObject M_TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 M_TSRMLS_CC, (char*)"%s : digest size is %d bytes, returned %d bytes", ce->name, DigestSize(), Z_STRLEN_P(zOutput));

        zval_ptr_dtor(&zOutput);
        throw false;
    }

    int length = std::min(static_cast<int>(digestSize), Z_STRLEN_P(zOutput));
    memcpy(digest, Z_STRVAL_P(zOutput), length);
    zval_ptr_dtor(&zOutput);
}

void MacProxy::CalculateDigest(byte *digest, const byte *input, size_t length) {
    zval *zInput    = makeZval(reinterpret_cast<const char*>(input), length);
    zval *zOutput   = call_user_method(m_macObject, m_funcnameCalculateDigest, zInput M_TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(zOutput)) {
        zval_ptr_dtor(&zInput);
        zval_ptr_dtor(&zOutput);
        throw false;
    } else if (DigestSize() != Z_STRLEN_P(zOutput)) {
        // bad returned digest size
        zend_class_entry *ce = zend_get_class_entry(m_macObject M_TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 M_TSRMLS_CC, (char*)"%s : digest size is %d bytes, returned %d bytes", ce->name, DigestSize(), Z_STRLEN_P(zOutput));

        zval_ptr_dtor(&zInput);
        zval_ptr_dtor(&zOutput);
        throw false;
    }

    memcpy(digest, Z_STRVAL_P(zOutput), Z_STRLEN_P(zOutput));
    zval_ptr_dtor(&zInput);
    zval_ptr_dtor(&zOutput);
}

void MacProxy::Update(const byte *input, size_t length) {
    zval *zInput    = makeZval(reinterpret_cast<const char*>(input), length);
    zval *zOutput   = call_user_method(m_macObject, m_funcnameUpdate, zInput M_TSRMLS_CC);

    zval_ptr_dtor(&zInput);
    zval_ptr_dtor(&zOutput);
}

void MacProxy::Final(byte *digest) {
    TruncatedFinal(digest, m_digestSize);
}

void MacProxy::Restart() {
    zval *zOutput = call_user_method(m_macObject, m_funcnameRestart M_TSRMLS_CC);
    zval_ptr_dtor(&zOutput);
}

void MacProxy::SetKey(const byte *key, size_t length, const CryptoPP::NameValuePairs &params)
{
    zval *zInput    = makeZval(reinterpret_cast<const char*>(key), length);
    zval *zOutput   = call_user_method(m_macObject, m_funcnameSetKey, zInput M_TSRMLS_CC);

    zval_ptr_dtor(&zInput);
    zval_ptr_dtor(&zOutput);
}

bool MacProxy::IsValidKeyLength(size_t n) const
{
    return const_cast<MacProxy*>(this)->IsValidKeyLength(n);
}

bool MacProxy::IsValidKeyLength(size_t n)
{
    zval *zKeySize  = makeZval(static_cast<long>(n));
    zval *output    = call_user_method(m_macObject, m_funcnameIsValidKeyLength, zKeySize M_TSRMLS_CC);
    bool isValid    = Z_BVAL_P(output);

    zval_ptr_dtor(&zKeySize);
    zval_ptr_dtor(&output);

    return isValid;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
