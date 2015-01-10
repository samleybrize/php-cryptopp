#include "../../php_cryptopp.h"
#include "../../exception/php_exception.h"
#include "../../utils/zval_utils.h"
#include "php_symmetric_transformation_interface.h"
#include "symmetric_transformation_proxy.h"
#include <zend_exceptions.h>
#include <exception>

SymmetricTransformationProxy::Base::Base(zval *symmetricTransformationObject, const char* processDataFuncname)
{
    // verify that symmetricTransformationObject is an instance of SymmetricTransformationInterface
    if (IS_OBJECT != Z_TYPE_P(symmetricTransformationObject) ||
            !instanceof_function(Z_OBJCE_P(symmetricTransformationObject), cryptopp_ce_SymmetricTransformationInterface)) {
        throw "SymmetricTransformationProxy expect a zval that holds an instance of Cryptopp\\SymmetricTransformationInterface";
    }

    // retrieve block size once
    zval *funcName      = makeZval("getBlockSize");
    zval *zBlockSize    = call_user_method(symmetricTransformationObject, funcName TSRMLS_CC);

    if (IS_LONG != Z_TYPE_P(zBlockSize)) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(symmetricTransformationObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : invalid block size returned", ce->name);

        zval_dtor(funcName);
        zval_dtor(zBlockSize);
        throw false;
    }

    m_blockSize = static_cast<unsigned int>(Z_LVAL_P(zBlockSize));
    zval_dtor(funcName);
    zval_dtor(zBlockSize);

    // create zvals with php method names
    m_processDataFuncname       = makeZval(processDataFuncname);
    m_funcnameIsValidKeyLength  = makeZval("isValidKeyLength");
    m_funcnameIsValidIvLength   = makeZval("isValidIvLength");
    m_funcnameSetKey            = makeZval("setKey");
    m_funcnameSetIv             = makeZval("setIv");
    m_funcnameRestart           = makeZval("restart");

    // hold symmetricTransformationObject
    m_symmetricTransformationObject = symmetricTransformationObject;
    Z_ADDREF_P(m_symmetricTransformationObject);
}

SymmetricTransformationProxy::Base::~Base()
{
    Z_DELREF_P(m_symmetricTransformationObject);
    zval_dtor(m_processDataFuncname);
    zval_dtor(m_funcnameIsValidKeyLength);
    zval_dtor(m_funcnameIsValidIvLength);
    zval_dtor(m_funcnameSetKey);
    zval_dtor(m_funcnameSetIv);
}

unsigned int SymmetricTransformationProxy::Base::MandatoryBlockSize() const
{
    return m_blockSize;
}

unsigned int SymmetricTransformationProxy::Base::OptimalBlockSize() const
{
    return m_blockSize;
}

void SymmetricTransformationProxy::Base::ProcessData(byte *outString, const byte *inString, size_t length)
{
    zval *input     = makeZval(reinterpret_cast<const char*>(inString), length);
    zval *output    = call_user_method(m_symmetricTransformationObject, m_processDataFuncname, input TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(output)) {
        zval_dtor(input);
        zval_dtor(output);
        throw false;
    }

    memcpy(outString, Z_STRVAL_P(output), Z_STRLEN_P(output));
    Z_DELREF_P(input);
    zval_dtor(output);
}

bool SymmetricTransformationProxy::Base::IsValidKeyLength(size_t n) const
{
    return const_cast<SymmetricTransformationProxy::Base*>(this)->IsValidKeyLength(n);
}

bool SymmetricTransformationProxy::Base::IsValidKeyLength(size_t n)
{
    zval *zKeySize  = makeZval(static_cast<long>(n));
    zval *output    = call_user_method(m_symmetricTransformationObject, m_funcnameIsValidKeyLength, zKeySize TSRMLS_CC);
    bool isValid    = Z_BVAL_P(output);

    Z_DELREF_P(zKeySize);
    zval_dtor(output);

    return isValid;
}

bool SymmetricTransformationProxy::Base::IsValidIvLength(size_t n)
{
    zval *zIvSize   = makeZval(static_cast<long>(n));
    zval *output    = call_user_method(m_symmetricTransformationObject, m_funcnameIsValidIvLength, zIvSize TSRMLS_CC);
    bool isValid    = Z_BVAL_P(output);

    Z_DELREF_P(zIvSize);
    zval_dtor(output);

    return isValid;
}

void SymmetricTransformationProxy::Base::SetKeyWithIV(const byte *key, size_t length, const byte *iv, size_t ivLength)
{
    zval *zKey      = makeZval(reinterpret_cast<const char*>(key), length);
    zval *zIv       = makeZval(reinterpret_cast<const char*>(iv), ivLength);
    zval *output;
    zval *output1   = call_user_method(m_symmetricTransformationObject, m_funcnameSetKey, zKey TSRMLS_CC);
    zval *output2   = call_user_method(m_symmetricTransformationObject, m_funcnameSetIv, zIv TSRMLS_CC);

    Z_DELREF_P(zKey);
    Z_DELREF_P(zIv);
    zval_dtor(output1);
    zval_dtor(output2);

    Restart();
}

void SymmetricTransformationProxy::Base::SetKey(const byte *key, size_t length, const CryptoPP::NameValuePairs &params)
{
    // retrieve IV from parameters
    CryptoPP::ConstByteArrayParameter ivWithLength;
    const byte *iv;
    size_t ivLength = 0;
    bool found      = false;

    try {
        found = params.GetValue(CryptoPP::Name::IV(), ivWithLength);
    } catch (const CryptoPP::NameValuePairs::ValueTypeMismatch &) {
    }

    if (found) {
        iv          = ivWithLength.begin();
        ivLength    = ivWithLength.size();
    } else {
        ThrowIfResynchronizable();
    }

    SetKeyWithIV(key, length, iv, ivLength);
}

void SymmetricTransformationProxy::Base::Restart()
{
    zval *output = call_user_method(m_symmetricTransformationObject, m_funcnameRestart TSRMLS_CC);
    zval_dtor(output);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
