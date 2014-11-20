#ifndef PHP_CRYPTOPP_H
#define PHP_CRYPTOPP_H

#define PHP_CRYPTOPP_EXTNAME "crypto++"
#define PHP_CRYPTOPP_VERSION "%ext_version%"
#define PHP_CRYPTOPP_NAMESPACE "Cryptopp"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
}

/* {{{ include crypto++ */
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptlib.h"
/* }}} */

/* {{{ declare module entry */
extern zend_module_entry cryptopp_module_entry;
#define phpext_cryptopp_ptr &cryptopp_module_entry;
/* }}} */

//%configure_inclusion%

#endif /* PHP_CRYPTOPP_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
