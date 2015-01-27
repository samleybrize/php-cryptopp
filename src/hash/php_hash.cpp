/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "src/php_cryptopp.h"
#include "php_hash.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

AlgoList hashAlgoList;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_Hash_getAlgos, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_Hash_getClassname, 0)
    ZEND_ARG_INFO(0, algoName)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class declaration */
zend_class_entry *cryptopp_ce_Hash;

static zend_function_entry cryptopp_methods_Hash[] = {
    PHP_ME(Cryptopp_Hash, getAlgos, arginfo_Hash_getAlgos, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(Cryptopp_Hash, getClassname, arginfo_Hash_getClassname, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};

void init_class_Hash(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "Hash", cryptopp_methods_Hash);
    cryptopp_ce_Hash            = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_Hash->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
}
/* }}} */

/* {{{ proto array Hash::getAlgos(void)
   Get the list of supported hash algorithms */
PHP_METHOD(Cryptopp_Hash, getAlgos) {
    hashAlgoList.getAlgoList(return_value);
}
/* }}} */

/* {{{ proto array Hash::getClassname(string algoName)
   Get the name of the class that implements a hash algorithm */
PHP_METHOD(Cryptopp_Hash, getClassname) {
    char *algoName      = NULL;
    int algoNameSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &algoName, &algoNameSize)) {
        return;
    }

    string algoNameStr(algoName, algoNameSize);
    string classname = hashAlgoList.getAlgoClass(algoNameStr);

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
