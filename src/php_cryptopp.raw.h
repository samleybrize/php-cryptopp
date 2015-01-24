/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_CRYPTOPP_H
#define PHP_CRYPTOPP_H

#define PHP_CRYPTOPP_EXTNAME "crypto++"
#define PHP_CRYPTOPP_VERSION "%ext_version%"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
}

#ifdef ZTS

#define M_TSRMLS_D  void ***m_tsrm_ls
#define M_TSRMLS_C  m_tsrm_ls
#define M_TSRMLS_CC , M_TSRMLS_C
#define SET_M_TSRMLS_C() M_TSRMLS_C = TSRMLS_C;

#else /* non ZTS */

#define M_TSRMLS_D
#define M_TSRMLS_C
#define M_TSRMLS_CC
#define SET_M_TSRMLS_C()

#endif /* ZTS */

/* {{{ include crypto++ */
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptlib.h>
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
