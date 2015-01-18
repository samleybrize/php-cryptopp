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
#include "php_hash_equals.h"

/* add PHP's hash_equals() function if not available */
#if PHP_API_VERSION < 20131106

ZEND_BEGIN_ARG_INFO(arginfo_hash_equals, 0)
    ZEND_ARG_INFO(0, known_string)
    ZEND_ARG_INFO(0, user_string)
ZEND_END_ARG_INFO()

const zend_function_entry hash_equals_function[] = {
    PHP_FE(hash_equals, arginfo_hash_equals)
    PHP_FE_END
};

/* {{{ proto bool hash_equals(string known_string, string user_string)
   Compares two strings using the same time whether they're equal or not.
   A difference in length will leak */
PHP_FUNCTION(hash_equals)
{
    zval *known_zval, *user_zval;
    char *known_str, *user_str;
    int result = 0, j;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &known_zval, &user_zval) == FAILURE) {
        return;
    }

    /* We only allow comparing string to prevent unexpected results. */
    if (Z_TYPE_P(known_zval) != IS_STRING) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Expected known_string to be a string, %s given", zend_zval_type_name(known_zval));
        RETURN_FALSE;
    }

    if (Z_TYPE_P(user_zval) != IS_STRING) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Expected user_string to be a string, %s given", zend_zval_type_name(user_zval));
        RETURN_FALSE;
    }

    if (Z_STRLEN_P(known_zval) != Z_STRLEN_P(user_zval)) {
        RETURN_FALSE;
    }

    known_str   = Z_STRVAL_P(known_zval);
    user_str    = Z_STRVAL_P(user_zval);

    /* This is security sensitive code. Do not optimize this for speed. */
    for (j = 0; j < Z_STRLEN_P(known_zval); j++) {
        result |= known_str[j] ^ user_str[j];
    }

    RETURN_BOOL(0 == result);
}
/* }}} */

#else

const zend_function_entry hash_equals_function[] = {
    PHP_FE_END
};

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
