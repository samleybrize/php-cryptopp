#!/bin/bash

if [ ! -f config.m4 ]; then
    echo "ERROR: cannot find 'config.m4' file"
    exit 1
fi

trim() {
    local var=$@
    var="${var#"${var%%[![:space:]]*}"}"   # remove leading whitespace characters
    var="${var%"${var##*[![:space:]]}"}"   # remove trailing whitespace characters
    echo -n "$var"
}

# config file list
BASH_FILE_LIST=""

BASH_FILE_LIST="$BASH_FILE_LIST src/hash/hash_interface_config.sh"
BASH_FILE_LIST="$BASH_FILE_LIST src/hash/md5_config.sh"
BASH_FILE_LIST="$BASH_FILE_LIST src/hash/sha1_config.sh"
BASH_FILE_LIST="$BASH_FILE_LIST src/hash/sha3_config.sh"

# retrieve PHP functions list added by this extension, and cpp file list
HAS_ERROR=0
PHP_FUNCTIONS=""
PHP_MINIT_STATEMENTS=""
SRC_FILE_LIST="src/cryptopp_test.cpp"
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
    PHP_FUNCTIONS="$PHP_FUNCTIONS ${FILE_OUTPUT[2]}"
    PHP_MINIT_STATEMENTS="$PHP_MINIT_STATEMENTS ${FILE_OUTPUT[3]}"
done

# build includes for header files
IFS=$' '
HEADER_FILE_INCLUDES=""

for i in $HEADER_FILE_LIST; do
    HEADER_FILE_INCLUDES="$HEADER_FILE_INCLUDES
#include \"$i\""
done

# write PHP function list to a header file
cat <<EOF > ./src/php_functions.h
#ifndef PHP_FUNCTIONS_H
#define PHP_FUNCTIONS_H

$HEADER_FILE_INCLUDES

#define CRYPTOPP_TEST_PHP_FUNCTIONS $PHP_FUNCTIONS
#define PHP_MINIT_STATEMENTS $PHP_MINIT_STATEMENTS

#endif

EOF

# print the list of source files to add
echo $SRC_FILE_LIST
