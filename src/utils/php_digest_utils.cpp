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
#include "php_digest_utils.h"

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_DigestUtils_equals, 0)
    ZEND_ARG_INFO(0, known_string)
    ZEND_ARG_INFO(0, user_string)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_DigestUtils;

static zend_function_entry cryptopp_methods_DigestUtils[] = {
    PHP_ME(Cryptopp_DigestUtils, equals, arginfo_DigestUtils_equals, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};

void init_class_DigestUtils(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "DigestUtils", cryptopp_methods_DigestUtils);
    cryptopp_ce_DigestUtils             = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_DigestUtils->ce_flags  |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
}
/* }}} */

// TODO
bool cryptoppDigestEquals(char *strKnown, int strKnownLength, char *strUser, int strUserLength) {
    if (strKnownLength != strUserLength) {
        return false;
    }

    int result = 0;
    int j;

    for (j = 0; j < strKnownLength; j++) {
        result |= strKnown[j] ^ strUser[j];
    }

    return 0 == result;
}

/* {{{ proto boolean DigestUtils::equals(string, string)
   Compares two strings using the same time whether they're equal or not. */
PHP_METHOD(Cryptopp_DigestUtils, equals) {
    zval *known_zval, *user_zval;
    int result = 0;

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

    result = cryptoppDigestEquals(Z_STRVAL_P(known_zval), Z_STRLEN_P(known_zval), Z_STRVAL_P(user_zval), Z_STRLEN_P(user_zval));
    RETURN_BOOL(result)
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
