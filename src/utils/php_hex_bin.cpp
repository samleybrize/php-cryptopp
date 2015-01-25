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
#include "php_hex_bin.h"

/* add PHP's hex2bin() function if not available */
#if PHP_API_VERSION < 20100412

/* {{{ php_hex2bin
*/
static char *php_hex2bin(const unsigned char *old, const size_t oldlen, size_t *newlen)
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

/* {{{ proto string hex2bin(string data)
   Converts the hex representation of data to binary */
PHP_FUNCTION(hex2bin)
{
    char *result, *data;
    size_t newlen;
    int datalen;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &datalen) == FAILURE) {
        return;
    }

    result = php_hex2bin((unsigned char *)data, datalen, &newlen);

    if (!result) {
        RETURN_FALSE;
    }

    RETURN_STRINGL(result, newlen, 0);
}
/* }}} */

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
