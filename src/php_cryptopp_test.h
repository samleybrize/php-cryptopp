#ifndef PHP_CRYPTOPP_TEST_H
#define PHP_CRYPTOPP_TEST_H

#define PHP_CRYPTOPP_TEST_EXTNAME "cryptopp_test"
#define PHP_CRYPTOPP_TEST_VERSION "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
}

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptlib.h"

extern zend_module_entry cryptopp_test_module_entry;
#define phpext_cryptopp_test_ptr &cryptopp_test_module_entry;

void outputByteArray(byte *byteArray, int size);

#endif

