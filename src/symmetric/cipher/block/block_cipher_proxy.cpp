#include "../../../php_cryptopp.h"
#include "../../../exception/php_exception.h"
#include "php_block_cipher_interface.h"
#include "block_cipher_proxy.h"
#include <zend_exceptions.h>
#include <exception>

BlockCipherProxy::Base::Base(zval *blockCipherObject, const char* processDataFuncname, const char *processBlockFuncname)
{
    // verify that blockCipherObject is an instance of BlockCipherInterface
    if (IS_OBJECT != Z_TYPE_P(blockCipherObject) ||
            !instanceof_function(Z_OBJCE_P(blockCipherObject), cryptopp_ce_BlockCipherInterface)) {
        throw "BlockCipherProxy expect a zval that holds an instance of Cryptopp\\BlockCipherInterface";
    }

    // retrieve block size once
    zval *funcName;
    zval *zBlockSize;
    MAKE_STD_ZVAL(funcName);
    MAKE_STD_ZVAL(zBlockSize);
    ZVAL_STRING(funcName, "getBlockSize", 1);
    call_user_function(NULL, &blockCipherObject, funcName, zBlockSize, 0, NULL TSRMLS_CC);

    if (IS_LONG != Z_TYPE_P(zBlockSize)) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(blockCipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : invalid block size returned", ce->name);

        zval_dtor(funcName);
        zval_dtor(zBlockSize);
        throw false;
    }

    m_blockSize = static_cast<unsigned int>(Z_LVAL_P(zBlockSize));
    zval_dtor(funcName);
    zval_dtor(zBlockSize);

    // retrieve algo name once
    zval *zName;
    MAKE_STD_ZVAL(funcName);
    MAKE_STD_ZVAL(zName);
    ZVAL_STRING(funcName, "getName", 1);
    call_user_function(NULL, &blockCipherObject, funcName, zName, 0, NULL TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(zName)) {
        ZVAL_STRING(zName, "User", 1);
    }

    m_name.assign(Z_STRVAL_P(zName), Z_STRLEN_P(zName));
    zval_dtor(funcName);
    zval_dtor(zName);

    // create zvals with php method names
    MAKE_STD_ZVAL(m_funcnameProcessData);
    MAKE_STD_ZVAL(m_funcnameProcessBlock);
    MAKE_STD_ZVAL(m_funcnameIsValidKeyLength);
    MAKE_STD_ZVAL(m_funcnameSetKey);
    ZVAL_STRING(m_funcnameProcessData, processDataFuncname, 1);
    ZVAL_STRING(m_funcnameProcessBlock, processBlockFuncname, 1);
    ZVAL_STRING(m_funcnameIsValidKeyLength, "isValidKeyLength", 1);
    ZVAL_STRING(m_funcnameSetKey, "setKey", 1);

    // hold blockCipherObject
    m_blockCipherObject = blockCipherObject;
    Z_ADDREF_P(m_blockCipherObject);
}

BlockCipherProxy::Base::~Base()
{
    Z_DELREF_P(m_blockCipherObject);
    zval_dtor(m_funcnameProcessData);
    zval_dtor(m_funcnameProcessBlock);
    zval_dtor(m_funcnameIsValidKeyLength);
    zval_dtor(m_funcnameSetKey);
}

unsigned int BlockCipherProxy::Base::BlockSize() const
{
    return m_blockSize;
}

bool BlockCipherProxy::Base::IsValidKeyLength(size_t n) const
{
    return const_cast<BlockCipherProxy::Base*>(this)->IsValidKeyLength(n);
}

bool BlockCipherProxy::Base::IsValidKeyLength(size_t n)
{
    zval *zKeySize;
    zval *output;
    MAKE_STD_ZVAL(zKeySize)
    MAKE_STD_ZVAL(output)
    ZVAL_LONG(zKeySize, static_cast<long>(n));
    call_user_function(NULL, &m_blockCipherObject, m_funcnameIsValidKeyLength, output, 1, &zKeySize TSRMLS_CC);

    bool isValid = Z_BVAL_P(output);

    Z_DELREF_P(zKeySize);
    zval_dtor(output);

    return isValid;
}

void BlockCipherProxy::Base::SetKey(const byte *key, size_t length, const CryptoPP::NameValuePairs &params)
{
    zval *zKey;
    zval *output;
    MAKE_STD_ZVAL(zKey)
    MAKE_STD_ZVAL(output)
    ZVAL_STRINGL(zKey, reinterpret_cast<const char*>(key), length, 1);
    call_user_function(NULL, &m_blockCipherObject, m_funcnameSetKey, output, 1, &zKey TSRMLS_CC);

    Z_DELREF_P(zKey);
    zval_dtor(output);
}

void BlockCipherProxy::Base::ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const
{
    const_cast<BlockCipherProxy::Base*>(this)->ProcessAndXorBlock(inBlock, xorBlock, outBlock);
}

void BlockCipherProxy::Base::ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock)
{
    // TODO optimize? (use of AdvancedProcessBlocks)
    // process block
    unsigned int blockSize = BlockSize();
    zval *zInBlock;
    zval *zProcessedBlock;
    MAKE_STD_ZVAL(zInBlock);
    MAKE_STD_ZVAL(zProcessedBlock);
    ZVAL_STRINGL(zInBlock, reinterpret_cast<const char*>(inBlock), blockSize, 1);
    call_user_function(NULL, &m_blockCipherObject, m_funcnameProcessBlock, zProcessedBlock, 1, &zInBlock TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(zProcessedBlock)) {
        Z_DELREF_P(zInBlock);
        zval_dtor(zProcessedBlock);
        throw false;
    }

    memcpy(outBlock, reinterpret_cast<byte*>(Z_STRVAL_P(zProcessedBlock)), blockSize);

    // xor block
    if (NULL != xorBlock) {
        for (int i = 0; i < blockSize; i++) {
            outBlock[i] ^= xorBlock[i];
        }
    }

    Z_DELREF_P(zInBlock);
    zval_dtor(zProcessedBlock);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
