#include "../../../php_cryptopp.h"
#include "php_stream_cipher.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

AlgoList streamCipherAlgoList;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_StreamCipher_getAlgos, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_StreamCipher_getClassname, 0)
    ZEND_ARG_INFO(0, algoName)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_StreamCipher;

static zend_function_entry cryptopp_methods_StreamCipher[] = {
    PHP_ME(Cryptopp_StreamCipher, getAlgos, arginfo_StreamCipher_getAlgos, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(Cryptopp_StreamCipher, getClassname, arginfo_StreamCipher_getClassname, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};

void init_class_StreamCipher(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "StreamCipher", cryptopp_methods_StreamCipher);
    cryptopp_ce_StreamCipher            = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_StreamCipher->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
}
/* }}} */

/* {{{ proto array StreamCipher::getAlgos(void)
   Get the list of supported stream ciphers */
PHP_METHOD(Cryptopp_StreamCipher, getAlgos) {
    array_init(return_value);
    vector<string> _algos = streamCipherAlgoList.getAlgoList();

    for (vector<string>::iterator it = _algos.begin(); it != _algos.end(); ++it) {
        add_next_index_string(return_value, it->c_str(), it->length());
    }
}
/* }}} */

/* {{{ proto array StreamCipher::getClassname(string algoName)
   Get the name of the class that implements a stream cipher */
PHP_METHOD(Cryptopp_StreamCipher, getClassname) {
    char *algoName      = NULL;
    int algoNameSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &algoName, &algoNameSize)) {
        return;
    }

    string algoNameStr(algoName, algoNameSize);
    string classname = streamCipherAlgoList.getAlgoClass(algoNameStr);

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
