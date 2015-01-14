
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "php_cryptopp.h"
#include "hash/php_hash.h"
#include "mac/php_mac.h"
#include "symmetric/cipher/authenticated/php_authenticated_symmetric_cipher.h"
#include "symmetric/cipher/block/php_block_cipher.h"
#include "symmetric/cipher/stream/php_stream_cipher.h"
#include "symmetric/mode/php_symmetric_mode.h"
#include "utils/php_hash_equals.h"
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

    vector<string> algoList;

    /* supported hash algos */
    algoList = hashAlgoList.getAlgoList();
    string supportedHashList;

    for (vector<string>::iterator it = algoList.begin(); it != algoList.end(); ++it) {
        supportedHashList.append(it->c_str());
        supportedHashList.append(" ");
    }

    /* supported MAC algos */
    algoList = macAlgoList.getAlgoList();
    string supportedMacList;

    for (vector<string>::iterator it = algoList.begin(); it != algoList.end(); ++it) {
        supportedMacList.append(it->c_str());
        supportedMacList.append(" ");
    }

    /* supported stream ciphers */
    algoList = streamCipherAlgoList.getAlgoList();
    string supportedStreamCipherList;

    for (vector<string>::iterator it = algoList.begin(); it != algoList.end(); ++it) {
        supportedStreamCipherList.append(it->c_str());
        supportedStreamCipherList.append(" ");
    }

    /* supported block ciphers */
    algoList = blockCipherAlgoList.getAlgoList();
    string supportedBlockCipherList;

    for (vector<string>::iterator it = algoList.begin(); it != algoList.end(); ++it) {
        supportedBlockCipherList.append(it->c_str());
        supportedBlockCipherList.append(" ");
    }

    /* supported authenticated ciphers */
    algoList = authenticatedSymmetricCipherAlgoList.getAlgoList();
    string supportedAuthenticatedCipherList;

    for (vector<string>::iterator it = algoList.begin(); it != algoList.end(); ++it) {
        supportedAuthenticatedCipherList.append(it->c_str());
        supportedAuthenticatedCipherList.append(" ");
    }

    /* supported cipher modes */
    vector<string> modeAlgoList = symmetricModeList.getAlgoList();
    string supportedModeList;

    for (vector<string>::iterator it = modeAlgoList.begin(); it != modeAlgoList.end(); ++it) {
        supportedModeList.append(it->c_str());
        supportedModeList.append(" ");
    }

    php_info_print_table_start();
    php_info_print_table_header(2, "Crypto++ support", "enabled");
    php_info_print_table_row(2, "Crypto++ module version", PHP_CRYPTOPP_VERSION);
    php_info_print_table_row(2, "Crypto++ library version", cryptopp_version_dotted);
    php_info_print_table_row(2, "Supported hash algorithms", supportedHashList.c_str());
    php_info_print_table_row(2, "Supported MAC algorithms", supportedMacList.c_str());
    php_info_print_table_row(2, "Supported stream ciphers", supportedStreamCipherList.c_str());
    php_info_print_table_row(2, "Supported block ciphers", supportedBlockCipherList.c_str());
    php_info_print_table_row(2, "Supported cipher modes", supportedModeList.c_str());
    php_info_print_table_row(2, "Supported authenticated encryption schemes", supportedAuthenticatedCipherList.c_str());
    php_info_print_table_end();
}
/* }}} */

/* {{{ Module struct */
zend_module_entry cryptopp_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_CRYPTOPP_EXTNAME,
    hash_equals_function,
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
