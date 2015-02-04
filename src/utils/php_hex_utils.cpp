/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/*
  +----------------------------------------------------------------------+
  | PHP Version 5 |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2014 The PHP Group |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license, |
  | that is bundled with this package in the file LICENSE, and is |
  | available through the world-wide-web at the following url: |
  | http://www.php.net/license/3_01.txt |
  | If you did not receive a copy of the PHP license and are unable to |
  | obtain it through the world-wide-web, please send a note to |
  | license@php.net so we can mail you a copy immediately. |
  +----------------------------------------------------------------------+
  | Author: Sara Golemon <pollita@php.net> |
  +----------------------------------------------------------------------+
*/

#include "src/php_cryptopp.h"
#include "php_hex_utils.h"

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_HexUtils_bin2hex, 0)
    ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_HexUtils_hex2bin, 0)
    ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_HexUtils;

static zend_function_entry cryptopp_methods_HexUtils[] = {
    PHP_ME(Cryptopp_HexUtils, bin2hex, arginfo_HexUtils_bin2hex, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(Cryptopp_HexUtils, hex2bin, arginfo_HexUtils_hex2bin, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};

void init_class_HexUtils(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "HexUtils", cryptopp_methods_HexUtils);
    cryptopp_ce_HexUtils            = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_HexUtils->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
}
/* }}} */

/* {{{ php_bin2hex */
static char hexconvtab[] = "0123456789abcdef";

static char *php_bin2hex(const unsigned char *old, const size_t oldlen, size_t *newlen) {
    register unsigned char *result = NULL;
    size_t i, j;
    result = (unsigned char *) safe_emalloc(oldlen, 2 * sizeof(char), 1);

    for (i = j = 0; i < oldlen; i++) {
        result[j++] = hexconvtab[old[i] >> 4];
        result[j++] = hexconvtab[old[i] & 15];
    }

    result[j] = '\0';

    if (newlen) {
        *newlen = oldlen * 2 * sizeof(char);
    }

    return (char *) result;
}
/* }}} */

/* {{{ php_hex2bin  */
static char *hex2bin(const unsigned char *old, const size_t oldlen, size_t *newlen)
{
    size_t target_length = oldlen >> 1;
    register unsigned char *str = (unsigned char *)safe_emalloc(target_length, sizeof(char), 1);
    size_t i, j;

    for (i = j = 0; i < target_length; i++) {
        char c = old[j++];

        if (c >= '0' && c <= '9') {
            str[i] = (c - '0') << 4;
        } else if (c >= 'a' && c <= 'f') {
            str[i] = (c - 'a' + 10) << 4;
        } else if (c >= 'A' && c <= 'F') {
            str[i] = (c - 'A' + 10) << 4;
        } else {
            efree(str);
            return NULL;
        }

        c = old[j++];

        if (c >= '0' && c <= '9') {
            str[i] |= c - '0';
        } else if (c >= 'a' && c <= 'f') {
            str[i] |= c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') {
            str[i] |= c - 'A' + 10;
        } else {
            efree(str);
            return NULL;
        }
    }

    str[target_length] = '\0';

    if (newlen) {
        *newlen = target_length;
    }

    return (char *)str;
}
/* }}} */

/* {{{ proto string HexUtils::bin2hex(string)
   Convert binary data into hexadecimal representation */
PHP_METHOD(Cryptopp_HexUtils, bin2hex) {
    char *result, *data;
    size_t newlen;
    int datalen;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &datalen) == FAILURE) {
        return;
    }

    result = php_bin2hex((unsigned char *)data, datalen, &newlen);

    if (!result) {
        RETURN_FALSE;
    }

    RETURN_STRINGL(result, newlen, 0);
}
/* }}} */

/* {{{ proto string HexUtils::hex2bin(string)
   Decodes a hexadecimally encoded binary string */
PHP_METHOD(Cryptopp_HexUtils, hex2bin) {
    char *result, *data;
    size_t newlen;
    int datalen;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &datalen) == FAILURE) {
        return;
    }

    result = hex2bin((unsigned char *)data, datalen, &newlen);

    if (!result) {
        RETURN_FALSE;
    }

    RETURN_STRINGL(result, newlen, 0);
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
