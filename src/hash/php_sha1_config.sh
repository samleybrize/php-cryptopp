#/bin/bash

test ! -f "$CRYPTOPP_DIR/sha.h" && echo "DISABLED" && exit 0

# cpp files to include
echo "src/hash/php_sha1.cpp"

# header files to include
echo "hash/php_sha1.h"

# declared PHP functions
echo "//"

# init functions
echo "CRYPTOPP_HASH_GET_INIT_CLASS_FUNC_CALL(HashSha1)"
