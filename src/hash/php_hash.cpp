#include "../php_cryptopp.h"
#include "php_hash.h"
#include <algorithm>
#include <string>
#include <vector>

// TODO
extern "C" {
#include <ext/standard/php_array.h>
}

// TODO
std::vector<std::string> hashAlgoList;
std::vector<std::string> hashClassList;

void addHashAlgo(const std::string algoName, const std::string hashClassname) {
    hashAlgoList.push_back(algoName);
    hashClassList.push_back(hashClassname);
}

std::string getHashAlgoClass(const std::string algoName) {
    std::vector<std::string>::iterator iterator = std::find(hashAlgoList.begin(), hashAlgoList.end(), algoName);

    if (iterator == hashAlgoList.end()) {
        // hash algorithm not found
        return "";
    } else {
        // hash algorithm found
        // return corresponding hash classname
        int pos = iterator - hashAlgoList.begin();
        return hashClassList[pos];
    }
}

// PHP class déclaration
// TODO arginfo
zend_class_entry *cryptopp_ce_Hash;

static zend_function_entry cryptopp_methods_Hash[] = {
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_Hash, getAlgos, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(PHP_CRYPTOPP_NAMESPACE_Hash, getClassname, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};

void init_class_Hash(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, PHP_CRYPTOPP_NAMESPACE, "Hash", cryptopp_methods_Hash);
    cryptopp_ce_Hash            = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_Hash->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
}

// PHP methods definitions
PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_Hash, getAlgos) {
    array_init(return_value);
    addHashAlgo("azerty", "dummy"); // TODO

    for (std::vector<std::string>::iterator it = hashAlgoList.begin(); it != hashAlgoList.end(); ++it) {
        add_next_index_string(return_value, it->c_str(), it->length());
    }

    // TODO sort
    CALL_PHP_FUNCTION1(sort, NULL, return_value)
//    PHP_FN(sort) (INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(PHP_CRYPTOPP_NAMESPACE_Hash, getClassname) {
    char *algoName      = NULL;
    int algoNameSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &algoName, &algoNameSize)) {
        return;
    }

    std::string classname = getHashAlgoClass(algoName);

    if (classname.empty()) {
        // return NULL if algo not found
        RETURN_NULL()
    } else {
        // return classname
        RETURN_STRING(classname.c_str(), 1)
    }
}
