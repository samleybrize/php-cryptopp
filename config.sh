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

# retrieve PHP functions list added by this extension, and cpp file list
HAS_ERROR=0
BASH_FILE_LIST=$(find src -name "*_config.sh")
PHP_FUNCTIONS=""
SRC_FILE_LIST="src/cryptopp_test.cpp"
IFS=$'\n'

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
    PHP_FUNCTIONS="$PHP_FUNCTIONS ${FILE_OUTPUT[1]}"
done

PHP_FUNCTIONS=$(trim $PHP_FUNCTIONS)

if [ "" = "$PHP_FUNCTIONS" ]; then
    echo "ERROR: no PHP functions found"
    exit 1
fi

# write PHP function list to a header file
cat <<EOF > ./src/php_functions.h
#ifndef PHP_FUNCTIONS_H
#define PHP_FUNCTIONS_H

#define CRYPTOPP_TEST_PHP_FUNCTIONS $PHP_FUNCTIONS

#endif

EOF

# print the list of source files to add
echo $SRC_FILE_LIST

