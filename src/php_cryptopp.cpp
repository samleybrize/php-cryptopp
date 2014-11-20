
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "php_cryptopp.h"
#include "hash/php_hash.h"
#include <ext/standard/info.h>
#include <string>
#include <vector>

using namespace std;

/* {{{ PHP_MINIT_FUNCTION */
static PHP_MINIT_FUNCTION(cryptopp) {
    PHP_MINIT_STATEMENTS
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
static PHP_MINFO_FUNCTION(cryptopp) {
    /* libcrypto++ version */
    char cryptopp_version[3];
    char cryptopp_version_dotted[6];
    sprintf(cryptopp_version, "%d", CRYPTOPP_VERSION);
    sprintf(cryptopp_version_dotted, "%c.%c.%c", cryptopp_version[0], cryptopp_version[1], cryptopp_version[2]);

    /* supported hash algos */
    vector<string> hashAlgoList = getHashAlgoList();
    string supportedHashList;

    for (vector<string>::iterator it = hashAlgoList.begin(); it != hashAlgoList.end(); ++it) {
        supportedHashList.append(it->c_str());
        supportedHashList.append(" ");
    }

    php_info_print_table_start();
    php_info_print_table_header(2, "Crypto++ support", "enabled");
    php_info_print_table_row(2, "Crypto++ module version", PHP_CRYPTOPP_VERSION);
    php_info_print_table_row(2, "Crypto++ library version", cryptopp_version_dotted);
    php_info_print_table_row(2, "Supported hash algos", supportedHashList.c_str());
    php_info_print_table_end();
}
/* }}} */

/* {{{ Module struct */
zend_module_entry cryptopp_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_CRYPTOPP_EXTNAME,
    NULL,
    PHP_MINIT(cryptopp),
    NULL,
    NULL,
    NULL,
    PHP_MINFO(cryptopp),
    PHP_CRYPTOPP_VERSION,
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CRYPTOPP
extern "C" {
ZEND_GET_MODULE(cryptopp)
}
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
