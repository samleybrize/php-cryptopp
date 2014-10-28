#/bin/bash

test ! -f "$CRYPTOPP_DIR/sha3.h" && echo "DISABLED" && exit 0

# cpp files to include
echo "src/hash/php_sha3.cpp"

# header files to include
echo "hash/php_sha3.h"

# declared PHP functions
echo "//"

# init functions
echo "init_classes_HashSha3(TSRMLS_C);"
