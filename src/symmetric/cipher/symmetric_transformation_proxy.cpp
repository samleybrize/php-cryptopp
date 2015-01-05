#include "../../php_cryptopp.h"
#include "../../exception/php_exception.h"
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
    zval *funcName;
    zval *zBlockSize;
    MAKE_STD_ZVAL(funcName);
    MAKE_STD_ZVAL(zBlockSize);
    ZVAL_STRING(funcName, "getBlockSize", 1);
    call_user_function(NULL, &symmetricTransformationObject, funcName, zBlockSize, 0, NULL TSRMLS_CC);

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
    MAKE_STD_ZVAL(m_processDataFuncname);
    MAKE_STD_ZVAL(m_funcnameIsValidKeyLength);
    MAKE_STD_ZVAL(m_funcnameSetKey);
    MAKE_STD_ZVAL(m_funcnameSetIv);
    ZVAL_STRING(m_processDataFuncname, processDataFuncname, 1);
    ZVAL_STRING(m_funcnameIsValidKeyLength, "isValidKeyLength", 1);
    ZVAL_STRING(m_funcnameSetKey, "setKey", 1);
    ZVAL_STRING(m_funcnameSetIv, "setIv", 1);

    // hold symmetricTransformationObject
    m_symmetricTransformationObject = symmetricTransformationObject;
    Z_ADDREF_P(m_symmetricTransformationObject);
}

SymmetricTransformationProxy::Base::~Base()
{
    Z_DELREF_P(m_symmetricTransformationObject);
    zval_dtor(m_processDataFuncname);
    zval_dtor(m_funcnameIsValidKeyLength);
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
    zval *input;
    zval *output;
    MAKE_STD_ZVAL(input)
    MAKE_STD_ZVAL(output)
    ZVAL_STRINGL(input, reinterpret_cast<const char*>(inString), length, 1);
    call_user_function(NULL, &m_symmetricTransformationObject, m_processDataFuncname, output, 1, &input TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(output)) {
        zval_dtor(input);
        zval_dtor(output);
        throw false;
    }

    memcpy(outString, Z_STRVAL_P(output), Z_STRLEN_P(output));
    zval_dtor(input);
    zval_dtor(output);
}

bool SymmetricTransformationProxy::Base::IsValidKeyLength(size_t n) const
{
    return const_cast<SymmetricTransformationProxy::Base*>(this)->IsValidKeyLength(n);
}

bool SymmetricTransformationProxy::Base::IsValidKeyLength(size_t n)
{
    zval *zKeySize;
    zval *output;
    MAKE_STD_ZVAL(zKeySize)
    MAKE_STD_ZVAL(output)
    ZVAL_LONG(zKeySize, static_cast<long>(n));
    call_user_function(NULL, &m_symmetricTransformationObject, m_funcnameIsValidKeyLength, output, 1, &zKeySize TSRMLS_CC);

    bool isValid = Z_BVAL_P(output);

    zval_dtor(zKeySize);
    zval_dtor(output);

    return isValid;
}

void SymmetricTransformationProxy::Base::SetKeyWithIV(const byte *key, size_t length, const byte *iv, size_t ivLength)
{
    zval *zKey;
    zval *zIv;
    zval *output;
    MAKE_STD_ZVAL(zKey)
    MAKE_STD_ZVAL(zIv)
    MAKE_STD_ZVAL(output)
    ZVAL_STRINGL(zKey, reinterpret_cast<const char*>(key), length, 1);
    ZVAL_STRINGL(zIv, reinterpret_cast<const char*>(iv), ivLength, 1);
    call_user_function(NULL, &m_symmetricTransformationObject, m_funcnameSetKey, output, 1, &zKey TSRMLS_CC);
    call_user_function(NULL, &m_symmetricTransformationObject, m_funcnameSetIv, output, 1, &zIv TSRMLS_CC);

    zval_dtor(zKey);
    zval_dtor(zIv);
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
