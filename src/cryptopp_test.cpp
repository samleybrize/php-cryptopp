#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "php_cryptopp_test.h"

#include "hash/php_md5.h"
#include "hash/php_sha1.h"

zend_function_entry cryptopp_test_functions[] = {
    PHP_FE(cryptopp_md5, NULL)
    PHP_FE(cryptopp_sha1, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry cryptopp_test_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_CRYPTOPP_TEST_EXTNAME,
    cryptopp_test_functions,
    NULL,
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

