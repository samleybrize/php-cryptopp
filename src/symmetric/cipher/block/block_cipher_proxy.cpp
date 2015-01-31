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
#include "php_block_cipher_interface.h"
#include "block_cipher_proxy.h"
#include <zend_exceptions.h>
#include <exception>

/* {{{ BlockCipherProxy::Base::Base */
BlockCipherProxy::Base::Base(zval *blockCipherObject, const char* processDataFuncname, const char *processBlockFuncname TSRMLS_DC)
{
    SET_M_TSRMLS_C()
    m_dataProcessEnabled = true;

    // verify that blockCipherObject is an instance of BlockCipherInterface
    if (IS_OBJECT != Z_TYPE_P(blockCipherObject) ||
            !instanceof_function(Z_OBJCE_P(blockCipherObject), cryptopp_ce_BlockCipherInterface TSRMLS_CC)) {
        throw "BlockCipherProxy expect a zval that holds an instance of Cryptopp\\BlockCipherInterface";
    }

    // retrieve block size once
    zval *funcName      = makeZval("getBlockSize");
    zval *zBlockSize    = call_user_method(blockCipherObject, funcName TSRMLS_CC);

    if (IS_LONG != Z_TYPE_P(zBlockSize) || Z_LVAL_P(zBlockSize) <= 0) {
        zend_class_entry *ce = zend_get_class_entry(blockCipherObject TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : invalid block size returned", ce->name);

        zval_ptr_dtor(&funcName);
        zval_ptr_dtor(&zBlockSize);
        throw false;
    }

    m_blockSize = static_cast<unsigned int>(Z_LVAL_P(zBlockSize));
    zval_ptr_dtor(&funcName);
    zval_ptr_dtor(&zBlockSize);

    // retrieve algo name once
    funcName    = makeZval("getName");
    zval *zName = call_user_method(blockCipherObject, funcName TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(zName)) {
        ZVAL_STRING(zName, "User", 1);
    }

    m_name.assign(Z_STRVAL_P(zName), Z_STRLEN_P(zName));
    zval_ptr_dtor(&funcName);
    zval_ptr_dtor(&zName);

    // create zvals with php method names
    m_funcnameProcessData       = makeZval(processDataFuncname);
    m_funcnameProcessBlock      = makeZval(processBlockFuncname);
    m_funcnameIsValidKeyLength  = makeZval("isValidKeyLength");
    m_funcnameSetKey            = makeZval("setKey");

    // hold blockCipherObject
    m_blockCipherObject = blockCipherObject;
    Z_ADDREF_P(m_blockCipherObject);
}
/* }}} */

/* {{{ BlockCipherProxy::Base::~Base */
BlockCipherProxy::Base::~Base()
{
    zval_ptr_dtor(&m_blockCipherObject);
    zval_ptr_dtor(&m_funcnameProcessData);
    zval_ptr_dtor(&m_funcnameProcessBlock);
    zval_ptr_dtor(&m_funcnameIsValidKeyLength);
    zval_ptr_dtor(&m_funcnameSetKey);
}
/* }}} */

/* {{{ BlockCipherProxy::Base::BlockSize */
unsigned int BlockCipherProxy::Base::BlockSize() const
{
    return m_blockSize;
}
/* }}} */

/* {{{ BlockCipherProxy::Base::IsValidKeyLength */
bool BlockCipherProxy::Base::IsValidKeyLength(size_t n) const
{
    return const_cast<BlockCipherProxy::Base*>(this)->IsValidKeyLength(n);
}
/* }}} */

/* {{{ BlockCipherProxy::Base::IsValidKeyLength */
bool BlockCipherProxy::Base::IsValidKeyLength(size_t n)
{
    zval *zKeySize  = makeZval(static_cast<long>(n));
    zval *output    = call_user_method(m_blockCipherObject, m_funcnameIsValidKeyLength, zKeySize M_TSRMLS_CC);
    bool isValid    = Z_BVAL_P(output);

    zval_ptr_dtor(&zKeySize);
    zval_ptr_dtor(&output);

    return isValid;
}
/* }}} */

/* {{{ BlockCipherProxy::Base::SetKey */
void BlockCipherProxy::Base::SetKey(const byte *key, size_t length, const CryptoPP::NameValuePairs &params)
{
    zval *zKey      = makeZval(reinterpret_cast<const char*>(key), length);
    zval *output    = call_user_method(m_blockCipherObject, m_funcnameSetKey, zKey M_TSRMLS_CC);

    zval_ptr_dtor(&zKey);
    zval_ptr_dtor(&output);
}
/* }}} */

/* {{{ BlockCipherProxy::Base::ProcessAndXorBlock */
void BlockCipherProxy::Base::ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const
{
    const_cast<BlockCipherProxy::Base*>(this)->ProcessAndXorBlock(inBlock, xorBlock, outBlock);
}
/* }}} */

/* {{{ BlockCipherProxy::Base::ProcessAndXorBlock */
void BlockCipherProxy::Base::ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock)
{
    // TODO optimize? (use of AdvancedProcessBlocks)

    unsigned int blockSize = BlockSize();

    if (!m_dataProcessEnabled) {
        // data process is disabled
        memcpy(outBlock, inBlock, blockSize);
        return;
    }

    // process block
    zval *zInBlock          = makeZval(reinterpret_cast<const char*>(inBlock), blockSize);
    zval *zProcessedBlock   = call_user_method(m_blockCipherObject, m_funcnameProcessBlock, zInBlock M_TSRMLS_CC);

    if (IS_STRING != Z_TYPE_P(zProcessedBlock)) {
        zval_ptr_dtor(&zInBlock);
        zval_ptr_dtor(&zProcessedBlock);
        throw false;
    }

    memcpy(outBlock, reinterpret_cast<byte*>(Z_STRVAL_P(zProcessedBlock)), blockSize);

    // xor block
    if (NULL != xorBlock) {
        for (int i = 0; i < blockSize; i++) {
            outBlock[i] ^= xorBlock[i];
        }
    }

    zval_ptr_dtor(&zInBlock);
    zval_ptr_dtor(&zProcessedBlock);
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
