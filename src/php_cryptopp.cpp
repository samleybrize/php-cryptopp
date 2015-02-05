
/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "php_cryptopp.h"
#include "php_init.h"
#include "src/hash/php_hash.h"
#include "src/mac/php_mac.h"
#include "src/symmetric/cipher/authenticated/php_authenticated_symmetric_cipher.h"
#include "src/symmetric/cipher/block/php_block_cipher.h"
#include "src/symmetric/cipher/stream/php_stream_cipher.h"
#include "src/symmetric/mode/php_symmetric_mode.h"
#include <ext/standard/info.h>
#include <string>
#include <vector>

using namespace std;

/* {{{ PHP_MINIT_FUNCTION */
static PHP_MINIT_FUNCTION(cryptopp) {
    PHP_MINIT_STATEMENTS

    REGISTER_NS_LONG_CONSTANT("Cryptopp", "LITTLE_ENDIAN", PHP_CRYPTOPP_LITTLE_ENDIAN, CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT("Cryptopp", "BIG_ENDIAN", PHP_CRYPTOPP_BIG_ENDIAN, CONST_PERSISTENT);

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
static string getAlgoList(vector<string> algoList) {
    string strAlgoList;

    for (vector<string>::iterator it = algoList.begin(); it != algoList.end(); ++it) {
        strAlgoList.append(it->c_str());
        strAlgoList.append(" ");
    }

    return strAlgoList;
}

static PHP_MINFO_FUNCTION(cryptopp) {
    /* libcrypto++ version */
    char cryptopp_version[3];
    char cryptopp_version_dotted[6];
    sprintf(cryptopp_version, "%d", CRYPTOPP_VERSION);
    sprintf(cryptopp_version_dotted, "%c.%c.%c", cryptopp_version[0], cryptopp_version[1], cryptopp_version[2]);

    string supportedHashList                = getAlgoList(hashAlgoList.getAlgoList());
    string supportedMacList                 = getAlgoList(macAlgoList.getAlgoList());
    string supportedStreamCipherList        = getAlgoList(streamCipherAlgoList.getAlgoList());
    string supportedBlockCipherList         = getAlgoList(blockCipherAlgoList.getAlgoList());
    string supportedAuthenticatedCipherList = getAlgoList(authenticatedSymmetricCipherAlgoList.getAlgoList());
    string supportedModeList                = getAlgoList(symmetricModeList.getAlgoList());

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
    php_cryptopp_functions,
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
