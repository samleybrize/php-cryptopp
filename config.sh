#!/bin/bash

if [ ! -f config.m4 ]; then
    echo "ERROR: cannot find 'config.m4' file"
    exit 1
fi

# config file list
BASH_FILE_LIST=""

BASH_FILE_LIST="$BASH_FILE_LIST src/hash/php_hash_config.sh"
BASH_FILE_LIST="$BASH_FILE_LIST src/hash/php_hash_interface_config.sh"
BASH_FILE_LIST="$BASH_FILE_LIST src/hash/php_md5_config.sh"
BASH_FILE_LIST="$BASH_FILE_LIST src/hash/php_sha1_config.sh"
BASH_FILE_LIST="$BASH_FILE_LIST src/hash/php_sha3_config.sh"

# retrieve PHP functions list added by this extension, and cpp file list
HAS_ERROR=0
PHP_MINIT_STATEMENTS=""
SRC_FILE_LIST="src/php_cryptopp.cpp"
HEADER_FILE_LIST=""

for i in $BASH_FILE_LIST; do
    # verify that the file is executable
    if [ ! -x $i ]; then
        echo "file $i must be executable"
        HAS_ERROR=1
        continue
    fi

    # execute it and retrieve output
    FILE_OUTPUT=($(CRYPTOPP_DIR=$CRYPTOPP_DIR $i))
    test "${FILE_OUTPUT[0]}" = "DISABLED" && continue

    SRC_FILE_LIST="$SRC_FILE_LIST ${FILE_OUTPUT[0]}"
    HEADER_FILE_LIST="$HEADER_FILE_LIST ${FILE_OUTPUT[1]}"
    PHP_MINIT_STATEMENTS="$PHP_MINIT_STATEMENTS ${FILE_OUTPUT[2]}"
done

# build includes for header files
HEADER_FILE_INCLUDES=""

for i in $HEADER_FILE_LIST; do
    HEADER_FILE_INCLUDES="$HEADER_FILE_INCLUDES
#include \"$i\""
done

# write PHP function list to a header file
CONFIGURE_INCLUSION="$HEADER_FILE_INCLUDES

#define PHP_MINIT_STATEMENTS $PHP_MINIT_STATEMENTS"

EXTENSION_VERSION="0.1"
EXTENSION_VERSION_TAG="\%ext_version\%"
CONFIGURE_INCLUSION_TAG="//%configure_inclusion%"
PHP_CRYPTOPP_H_RAW=$(cat src/php_cryptopp.raw.h)
PHP_CRYPTOPP_H_RAW=${PHP_CRYPTOPP_H_RAW/$CONFIGURE_INCLUSION_TAG/$CONFIGURE_INCLUSION}
PHP_CRYPTOPP_H_RAW=${PHP_CRYPTOPP_H_RAW/$EXTENSION_VERSION_TAG/$EXTENSION_VERSION}
echo "$PHP_CRYPTOPP_H_RAW" > src/php_cryptopp.h

# print the list of source files to add
echo $SRC_FILE_LIST
