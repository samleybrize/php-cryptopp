#ifndef PHP_CRYPTOPP_H
#define PHP_CRYPTOPP_H

#define PHP_CRYPTOPP_EXTNAME "cryptopp"
#define PHP_CRYPTOPP_VERSION "%ext_version%"
#define PHP_CRYPTOPP_NAMESPACE "CryptoPP"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
}

// include crypto++
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptlib.h"

// declare module entry
extern zend_module_entry cryptopp_module_entry;
#define phpext_cryptopp_ptr &cryptopp_module_entry;

//%configure_inclusion%

#endif
