#include "src/php_cryptopp.h"
#include "src/exception/php_exception.h"
#include "php_padding_interface.h"
#include "php_pkcs7.h"
#include <algorithm>
#include <math.h>
#include <zend_exceptions.h>

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_PaddingPkcs7;

static zend_function_entry cryptopp_methods_PaddingPkcs7[] = {
    PHP_ME(Cryptopp_PaddingPkcs7, pad, arginfo_PaddingInterface_pad, ZEND_ACC_PUBLIC)
    PHP_ME(Cryptopp_PaddingPkcs7, unpad, arginfo_PaddingInterface_unpad, ZEND_ACC_PUBLIC)
    PHP_ME(Cryptopp_PaddingPkcs7, canPad, arginfo_PaddingInterface_canPad, ZEND_ACC_PUBLIC)
    PHP_ME(Cryptopp_PaddingPkcs7, canUnpad, arginfo_PaddingInterface_canUnpad, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

void init_class_PaddingPkcs7(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "PaddingPkcs7", cryptopp_methods_PaddingPkcs7);
    cryptopp_ce_PaddingPkcs7 = zend_register_internal_class(&ce TSRMLS_CC);

    zend_class_implements(cryptopp_ce_PaddingPkcs7 TSRMLS_CC, 1, cryptopp_ce_PaddingInterface);
}
/* }}} */

/* {{{ proto string|false PaddingPkcs7::pad(string data, int blockSize)
   Pad data */
PHP_METHOD(Cryptopp_PaddingPkcs7, pad) {
    char *data;
    int dataLength;
    long blockSize;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &data, &dataLength, &blockSize)) {
        RETURN_FALSE;
    }

    byte *plain         = reinterpret_cast<byte*>(data);
    long alignedSize    = ceil(static_cast<double>(dataLength) / static_cast<double>(blockSize)) * blockSize;

    if (blockSize < 1) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : block size cannot be lower than 1, %d given", cryptopp_ce_PaddingPkcs7->name, blockSize);
        RETURN_FALSE
    } else if (blockSize > 256) {
        // PKCS7 does not handle block sizes higher than 256
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : PKCS #7 padding does not handle block sizes higher than 256", cryptopp_ce_PaddingPkcs7->name);
        RETURN_FALSE
    } else if (alignedSize == dataLength) {
        // if data size is a multiple of block size, pad on an additional block size
        alignedSize += blockSize;
    }

    // pad
    const byte pad = byte(alignedSize - dataLength);
    byte padded[alignedSize];
    memcpy(padded, plain, dataLength);
    memset(padded + dataLength, pad, alignedSize - dataLength);

    char *returnData = reinterpret_cast<char*>(padded);
    RETURN_STRINGL(returnData, alignedSize, 1)
}
/* }}} */

/* {{{ proto string|false PaddingPkcs7::unpad(string data, int blockSize)
   Unpad data */
PHP_METHOD(Cryptopp_PaddingPkcs7, unpad) {
    char *data;
    int dataLength;
    long blockSize;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &data, &dataLength, &blockSize)) {
        RETURN_FALSE;
    }

    byte *padded = reinterpret_cast<byte*>(data);

    if (blockSize < 1) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : block size cannot be lower than 1, %d given", cryptopp_ce_PaddingPkcs7->name, blockSize);
        RETURN_FALSE
    } else if (blockSize > 256) {
        // PKCS7 does not handle block sizes higher than 256
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : PKCS #7 padding does not handle block sizes higher than 256", cryptopp_ce_PaddingPkcs7->name);
        RETURN_FALSE
    } else if (0 != dataLength % blockSize) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : data length is not a multiple of block size (block size is %d, data size is %d)", cryptopp_ce_PaddingPkcs7->name, blockSize, dataLength);
        RETURN_FALSE
    }

    // retrieve the pad character
    byte pad = padded[dataLength - 1];

    if (pad < 1 || pad > blockSize || std::find_if(padded + dataLength - pad, padded + dataLength, std::bind2nd(std::not_equal_to<byte>(), pad)) != padded + dataLength) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : invalid PKCS #7 block padding found", cryptopp_ce_PaddingPkcs7->name);
        RETURN_FALSE
    }

    // unpad
    long length = dataLength - pad;
    byte unpadded[length];
    memcpy(unpadded, padded, length);

    char *plain = reinterpret_cast<char*>(padded);
    RETURN_STRINGL(plain, length, 1)
}
/* }}} */

/* {{{ proto boolean PaddingPkcs7::canPad(void)
   Indicates if this padding scheme can pad data */
PHP_METHOD(Cryptopp_PaddingPkcs7, canPad) {
    RETURN_TRUE
}
/* }}} */

/* {{{ proto boolean PaddingPkcs7::canUnpad(void)
   Indicates if this padding scheme can unpad data */
PHP_METHOD(Cryptopp_PaddingPkcs7, canUnpad) {
    RETURN_TRUE
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
