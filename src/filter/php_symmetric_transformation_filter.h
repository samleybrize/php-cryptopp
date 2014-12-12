#ifndef PHP_SYMMETRIC_TRANSFORMATION_FILTER_H
#define PHP_SYMMETRIC_TRANSFORMATION_FILTER_H

#include "../php_cryptopp.h"
#include <filters.h>
#include <string>

void init_class_SymmetricTransformationFilter(TSRMLS_D);

/* {{{ fork of CryptoPP::StreamTransformationFilter to support padding schemes as objects */
class SymmetricTransformationFilter : public CryptoPP::FilterWithBufferedInput, public CryptoPP::BlockPaddingSchemeDef, private CryptoPP::FilterPutSpaceHelper
{
public:
    SymmetricTransformationFilter(CryptoPP::StreamTransformation &cipher, zval *paddingObject, bool cipherMustBeDestructed);
    ~SymmetricTransformationFilter();
    std::string AlgorithmName() const {return m_cipher.AlgorithmName();}

protected:
    bool PaddingObjectCanUnpad();
    void InitializeDerivedAndReturnNewSizes(const CryptoPP::NameValuePairs &parameters, size_t &firstSize, size_t &blockSize, size_t &lastSize);
    void FirstPut(const byte *inString);
    void NextPutMultiple(const byte *inString, size_t length);
    void NextPutModifiable(byte *inString, size_t length);
    void LastPut(const byte *inString, size_t length);

    CryptoPP::StreamTransformation &m_cipher;
    unsigned int m_optimalBufferSize;
    zval *m_paddingObject;
    bool m_cipherMustBeDestructed;
};
/* }}} */

/* {{{ get the pointer to the native stf encryptor object of the php class */
#define CRYPTOPP_SYMMETRIC_TRANSFORMATION_FILTER_GET_ENCRYPTOR_PTR(ptrName)    \
    ptrName = getCryptoppSymmetricTransformationFilterEncryptorPtr(getThis() TSRMLS_CC); \
                                                                            \
    if (NULL == ptrName) {                                                  \
        RETURN_FALSE;                                                       \
    }                                                                       \
/* }}} */

/* {{{ get the pointer to the native stf decryptor object of the php class */
#define CRYPTOPP_SYMMETRIC_TRANSFORMATION_FILTER_GET_DECRYPTOR_PTR(ptrName)    \
    ptrName = getCryptoppSymmetricTransformationFilterDecryptorPtr(getThis() TSRMLS_CC); \
                                                                            \
    if (NULL == ptrName) {                                                  \
        RETURN_FALSE;                                                       \
    }                                                                       \
/* }}} */

/* {{{ object creation related stuff */
struct SymmetricTransformationFilterContainer {
    zend_object std;
    SymmetricTransformationFilter *stfEncryptor;
    SymmetricTransformationFilter *stfDecryptor;
};

extern zend_object_handlers SymmetricTransformationFilter_object_handlers;
void SymmetricTransformationFilter_free_storage(void *object TSRMLS_DC);
zend_object_value SymmetricTransformationFilter_create_handler(zend_class_entry *type TSRMLS_DC);
/* }}} */

/* {{{ methods declarations */
PHP_METHOD(Cryptopp_SymmetricTransformationFilter, __construct);
PHP_METHOD(Cryptopp_SymmetricTransformationFilter, __sleep);
PHP_METHOD(Cryptopp_SymmetricTransformationFilter, __wakeup);
PHP_METHOD(Cryptopp_SymmetricTransformationFilter, getCipher);
PHP_METHOD(Cryptopp_SymmetricTransformationFilter, encryptString);
PHP_METHOD(Cryptopp_SymmetricTransformationFilter, decryptString);
/* }}} */

#endif /* PHP_SYMMETRIC_TRANSFORMATION_FILTER_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
