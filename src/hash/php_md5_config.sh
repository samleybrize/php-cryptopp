#!/bin/bash

test ! -f "$CRYPTOPP_DIR/md5.h" && echo "DISABLED" && exit 0

# cpp files to include
echo "src/hash/php_md5.cpp"

# header files to include
echo "hash/php_md5.h"

# declared PHP functions
#echo "PHP_FE(cryptopp_md5, NULL)"
echo "//"

# init functions
echo "init_class_HashMd5(TSRMLS_C);"
