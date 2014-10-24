#/bin/bash

test ! -f "$CRYPTOPP_DIR/sha.h" && echo "DISABLED" && exit 0

# cpp files to include
echo "src/hash/sha1.cpp"

# header files to include
echo "hash/php_sha1.h"

# declared PHP functions
#echo "PHP_FE(cryptopp_sha1, NULL)"
echo "//"

# init functions
echo "init_class_HashSha1(TSRMLS_C);"

