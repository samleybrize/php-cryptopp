/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "src/php_cryptopp.h"
#include "php_authenticated_symmetric_cipher.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

AlgoList authenticatedSymmetricCipherAlgoList;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipher_getAlgos, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_AuthenticatedSymmetricCipher_getClassname, 0)
    ZEND_ARG_INFO(0, algoName)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_AuthenticatedSymmetricCipher;

static zend_function_entry cryptopp_methods_AuthenticatedSymmetricCipher[] = {
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipher, getAlgos, arginfo_AuthenticatedSymmetricCipher_getAlgos, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipher, getClassname, arginfo_AuthenticatedSymmetricCipher_getClassname, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};

void init_class_AuthenticatedSymmetricCipher(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "AuthenticatedSymmetricCipher", cryptopp_methods_AuthenticatedSymmetricCipher);
    cryptopp_ce_AuthenticatedSymmetricCipher            = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_AuthenticatedSymmetricCipher->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
}
/* }}} */

/* {{{ proto array AuthenticatedSymmetricCipher::getAlgos(void)
   Get the list of supported authenticated symmetric ciphers */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipher, getAlgos) {
    authenticatedSymmetricCipherAlgoList.getAlgoList(return_value);
}
/* }}} */

/* {{{ proto array AuthenticatedSymmetricCipher::getClassname(string algoName)
   Get the name of the class that implements an authenticated symmetric cipher */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipher, getClassname) {
    char *algoName      = NULL;
    int algoNameSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &algoName, &algoNameSize)) {
        return;
    }

    string algoNameStr(algoName, algoNameSize);
    string classname = authenticatedSymmetricCipherAlgoList.getAlgoClass(algoNameStr);

    if (classname.empty()) {
        // return NULL if algo not found
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
