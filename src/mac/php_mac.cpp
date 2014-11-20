#include "../php_cryptopp.h"
#include "php_mac.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

/* {{{ mac algo list, and corresponding PHP classes */
vector<string> macAlgoList;
vector<string> macClassList;

void addMacAlgo(const string algoName, const string macClassname) {
    macAlgoList.push_back(algoName);
    macClassList.push_back(macClassname);
}

vector<string> getMacAlgoList() {
    vector<string> _algos(macAlgoList);
    sort(_algos.begin(), _algos.end());
    return _algos;
}

string getMacAlgoClass(const string &algoName) {
    vector<string>::iterator iterator = find(macAlgoList.begin(), macAlgoList.end(), algoName);

    if (iterator == macAlgoList.end()) {
        // mac algorithm not found
        return "";
    } else {
        // mac algorithm found
        // return corresponding mac classname
        int pos = iterator - macAlgoList.begin();
        return macClassList[pos];
    }
}
/* }}} */

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_Mac_getAlgos, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_Mac_getClassname, 0)
    ZEND_ARG_INFO(0, algoName)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_Mac;

static zend_function_entry cryptopp_methods_Mac[] = {
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_Mac, getAlgos, arginfo_Mac_getAlgos, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_Mac, getClassname, arginfo_Mac_getClassname, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};

void init_class_Mac(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, PHP_CRYPTOPP_NAMESPACE, "Mac", cryptopp_methods_Mac);
    cryptopp_ce_Mac             = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_Mac->ce_flags  |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
}
/* }}} */

/* {{{ proto array Mac::getAlgos(void)
   Get the list of supported mac algorithms */
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_Mac, getAlgos) {
    array_init(return_value);
    vector<string> _algos = getMacAlgoList();

    for (vector<string>::iterator it = _algos.begin(); it != _algos.end(); ++it) {
        add_next_index_string(return_value, it->c_str(), it->length());
    }
}
/* }}} */

/* {{{ proto array Mac::getClassname(string algoName)
   Get the the name of the class that implements a mac algorithm */
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_Mac, getClassname) {
    char *algoName      = NULL;
    int algoNameSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &algoName, &algoNameSize)) {
        return;
    }

    string algoNameStr(algoName, algoNameSize);
    string classname = getMacAlgoClass(algoNameStr);

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
