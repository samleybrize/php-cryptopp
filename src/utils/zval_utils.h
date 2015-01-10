#ifndef PHP_ZVAL_UTILS_H
#define PHP_ZVAL_UTILS_H

#include "../php_cryptopp.h"

/* {{{ create a zval */
inline zval *makeZval(const char *str) {
    zval *output;
    MAKE_STD_ZVAL(output);
    ZVAL_STRING(output, str, 1);

    return output;
}

inline zval *makeZval(const char *str, int length) {
    zval *output;
    MAKE_STD_ZVAL(output);
    ZVAL_STRINGL(output, str, length, 1);

    return output;
}

inline zval *makeZval(long number) {
    zval *output;
    MAKE_STD_ZVAL(output);
    ZVAL_LONG(output, number);

    return output;
}
/* }}} */

/* {{{ call a php object method */
inline zval *call_user_method(zval *object, zval *methodName, int numArgs, zval **args TSRMLS_DC) {
    zval *output;
    MAKE_STD_ZVAL(output);
    call_user_function(NULL, &object, methodName, output, numArgs, args TSRMLS_CC);

    return output;
}

inline zval *call_user_method(zval *object, zval *methodName TSRMLS_DC) {
    return call_user_method(object, methodName, 0, NULL TSRMLS_CC);
}

inline zval *call_user_method(zval *object, zval *methodName, zval *arg1 TSRMLS_DC) {
    return call_user_method(object, methodName, 1, &arg1 TSRMLS_CC);
}

inline zval *call_user_method(zval *object, zval *methodName, zval *arg1, zval *arg2 TSRMLS_DC) {
    zval *args[2];
    args[0] = arg1;
    args[1] = arg2;

    return call_user_method(object, methodName, 2, args TSRMLS_CC);
}

inline zval *call_user_method(zval *object, zval *methodName, zval *arg1, zval *arg2, zval *arg3 TSRMLS_DC) {
    zval *args[3];
    args[0] = arg1;
    args[1] = arg2;
    args[2] = arg3;

    return call_user_method(object, methodName, 3, args TSRMLS_CC);
}
/* }}} */

#endif /* PHP_ZVAL_UTILS_H */
