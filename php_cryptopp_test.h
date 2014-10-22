#ifndef PHP_CRYPTOPP_TEST
#define PHP_CRYPTOPP_TEST

#define PHP_CRYPTOPP_TEST_EXTNAME "cryptopp_test"
#define PHP_CRYPTOPP_TEST_VERSION "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
}

extern zend_module_entry cryptopp_test_module_entry;
#define phpext_cryptopp_test_ptr &cryptopp_test_module_entry;

PHP_FUNCTION(cryptopp_md5);
PHP_FUNCTION(cryptopp_sha1);

#endif

