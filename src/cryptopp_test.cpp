#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "php_functions.h"
#include "php_cryptopp_test.h"

zend_function_entry cryptopp_test_functions[] = {
    CRYPTOPP_TEST_PHP_FUNCTIONS
    {NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(cryptopp_test) {
    PHP_MINIT_STATEMENTS
}

zend_module_entry cryptopp_test_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_CRYPTOPP_TEST_EXTNAME,
    cryptopp_test_functions,
    PHP_MINIT(cryptopp_test),
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_CRYPTOPP_TEST_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_CRYPTOPP_TEST
extern "C" {
ZEND_GET_MODULE(cryptopp_test)
}
#endif

void outputByteArray(byte *byteArray, int size) {
    for (int i = 0; i < size; i++) {
        php_printf("%ld:", byteArray[i]);
    }

    php_printf("\n");
}

