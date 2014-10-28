#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "php_functions.h"
#include "php_cryptopp.h"

zend_function_entry cryptopp_functions[] = {
    CRYPTOPP_PHP_FUNCTIONS
    PHP_FE_END
};

PHP_MINIT_FUNCTION(cryptopp) {
    PHP_MINIT_STATEMENTS
	return SUCCESS;
}

zend_module_entry cryptopp_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_CRYPTOPP_EXTNAME,
    cryptopp_functions,
    PHP_MINIT(cryptopp),
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_CRYPTOPP_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_CRYPTOPP
extern "C" {
ZEND_GET_MODULE(cryptopp)
}
#endif
