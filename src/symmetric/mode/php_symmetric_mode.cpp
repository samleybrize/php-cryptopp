/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "src/php_cryptopp.h"
#include "php_symmetric_mode.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

AlgoList symmetricModeList;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_SymmetricMode_getModes, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricMode_getClassname, 0)
    ZEND_ARG_INFO(0, modeName)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_SymmetricMode;

static zend_function_entry cryptopp_methods_SymmetricMode[] = {
    PHP_ME(Cryptopp_SymmetricMode, getModes, arginfo_SymmetricMode_getModes, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(Cryptopp_SymmetricMode, getClassname, arginfo_SymmetricMode_getClassname, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};

void init_class_SymmetricMode(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "SymmetricMode", cryptopp_methods_SymmetricMode);
    cryptopp_ce_SymmetricMode               = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_SymmetricMode->ce_flags    |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
}
/* }}} */

/* {{{ proto array SymmetricMode::getModes(void)
   Get the list of supported symmetric modes */
PHP_METHOD(Cryptopp_SymmetricMode, getModes) {
    symmetricModeList.getAlgoList(return_value);
}
/* }}} */

/* {{{ proto array SymmetricMode::getClassname(string modeName)
   Get the name of the class that implements a symmetric mode */
PHP_METHOD(Cryptopp_SymmetricMode, getClassname) {
    char *modeName      = NULL;
    int modeNameSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &modeName, &modeNameSize)) {
        return;
    }

    string modeNameStr(modeName, modeNameSize);
    string classname = symmetricModeList.getAlgoClass(modeNameStr);

    if (classname.empty()) {
        // return NULL if mode not found
        RETURN_NULL()
    } else {
        // return classname
        RETURN_STRING(classname.c_str(), 1)
    }
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
