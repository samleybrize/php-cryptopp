#include <string>
#include <php.h>
#include "php_cryptopp_test.h"
#include "aes.h"

zend_function_entry cryptopp_test_functions[] = {
    PHP_FE(cryptopp_test, NULL)
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

ZEND_GET_MODULE(cryptopp_test)

PHP_FUNCTION(cryptopp_test) {
    php_printf("aes block size : " + std::to_string(CryptoPP::AES::BLOCKSIZE) + "\n");
}

