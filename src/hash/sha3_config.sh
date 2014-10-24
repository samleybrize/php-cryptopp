#/bin/bash

test ! -f "$CRYPTOPP_DIR/sha3.h" && echo "DISABLED" && exit 0

echo "src/hash/sha3.cpp"
echo "hash/php_sha3.h"
echo "PHP_FE(cryptopp_sha3_256, NULL)"

