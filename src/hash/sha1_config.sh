#/bin/bash

test ! -f "$CRYPTOPP_DIR/sha.h" && echo "DISABLED" && exit 0

echo "src/hash/sha1.cpp"
echo "PHP_FE(cryptopp_sha1, NULL)"

