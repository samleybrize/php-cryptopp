#include "../../php_cryptopp.h"
#include "php_symmetric_cipher.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

/* {{{ symmetric cipher algo list, and corresponding PHP classes */
vector<string> cipherAlgoList;
vector<string> cipherClassList;

void addSymmetricCipherAlgo(const string algoName, const string cipherClassname) {
    cipherAlgoList.push_back(algoName);
    cipherClassList.push_back(cipherClassname);
}

vector<string> getSymmetricCipherAlgoList() {
    vector<string> _algos(cipherAlgoList);
    sort(_algos.begin(), _algos.end());
    return _algos;
}

string getSymmetricCipherAlgoClass(const string &algoName) {
    vector<string>::iterator iterator = find(cipherAlgoList.begin(), cipherAlgoList.end(), algoName);

    if (iterator == cipherAlgoList.end()) {
        // symmetric cipher not found
        return "";
    } else {
        // symmetric cipher found
        // return corresponding symmetric cipher classname
        int pos = iterator - cipherAlgoList.begin();
        return cipherClassList[pos];
    }
}
/* }}} */

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_SymmetricCipher_getAlgos, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricCipher_getClassname, 0)
    ZEND_ARG_INFO(0, algoName)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_SymmetricCipher;

static zend_function_entry cryptopp_methods_SymmetricCipher[] = {
    PHP_ME(Cryptopp_SymmetricCipher, getAlgos, arginfo_SymmetricCipher_getAlgos, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(Cryptopp_SymmetricCipher, getClassname, arginfo_SymmetricCipher_getClassname, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};

void init_class_SymmetricCipher(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "SymmetricCipher", cryptopp_methods_SymmetricCipher);
    cryptopp_ce_SymmetricCipher             = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_SymmetricCipher->ce_flags  |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
}
/* }}} */

/* {{{ proto array SymmetricCipher::getAlgos(void)
   Get the list of supported symmetric ciphers */
PHP_METHOD(Cryptopp_SymmetricCipher, getAlgos) {
    array_init(return_value);
    vector<string> _algos = getSymmetricCipherAlgoList();

    for (vector<string>::iterator it = _algos.begin(); it != _algos.end(); ++it) {
        add_next_index_string(return_value, it->c_str(), it->length());
    }
}
/* }}} */

/* {{{ proto array SymmetricCipher::getClassname(string algoName)
   Get the the name of the class that implements a symmetric cipher */
PHP_METHOD(Cryptopp_SymmetricCipher, getClassname) {
    char *algoName      = NULL;
    int algoNameSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &algoName, &algoNameSize)) {
        return;
    }

    string algoNameStr(algoName, algoNameSize);
    string classname = getSymmetricCipherAlgoClass(algoNameStr);

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
