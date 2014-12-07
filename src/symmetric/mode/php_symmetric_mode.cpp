#include "../../php_cryptopp.h"
#include "php_symmetric_mode.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

/* {{{ symmetric mode algo list, and corresponding PHP classes */
static vector<string> modeNameList;
static vector<string> modeClassList;

void addSymmetricMode(const string algoName, const string modeClassname) {
    modeNameList.push_back(algoName);
    modeClassList.push_back(modeClassname);
}

vector<string> getSymmetricModeList() {
    vector<string> _algos(modeNameList);
    sort(_algos.begin(), _algos.end());
    return _algos;
}

string getSymmetricModeClass(const string &algoName) {
    vector<string>::iterator iterator = find(modeNameList.begin(), modeNameList.end(), algoName);

    if (iterator == modeNameList.end()) {
        // symmetric mode not found
        return "";
    } else {
        // symmetric mode found
        // return corresponding symmetric mode classname
        int pos = iterator - modeNameList.begin();
        return modeClassList[pos];
    }
}
/* }}} */

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_SymmetricMode_getModes, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_SymmetricMode_getClassname, 0)
    ZEND_ARG_INFO(0, algoName)
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
    array_init(return_value);
    vector<string> _modes = getSymmetricModeList();

    for (vector<string>::iterator it = _modes.begin(); it != _modes.end(); ++it) {
        add_next_index_string(return_value, it->c_str(), it->length());
    }
}
/* }}} */

/* {{{ proto array SymmetricMode::getClassname(string algoName)
   Get the the name of the class that implements a symmetric mode */
PHP_METHOD(Cryptopp_SymmetricMode, getClassname) {
    char *modeName      = NULL;
    int modeNameSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &modeName, &modeNameSize)) {
        return;
    }

    string modeNameStr(modeName, modeNameSize);
    string classname = getSymmetricModeClass(modeNameStr);

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
