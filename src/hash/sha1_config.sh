#/bin/bash

test ! -f "$CRYPTOPP_DIR/sha.h" && echo "DISABLED" && exit 0

echo "src/hash/sha1.cpp"
echo "hash/php_sha1.h"
echo "PHP_FE(cryptopp_sha1, NULL)"

