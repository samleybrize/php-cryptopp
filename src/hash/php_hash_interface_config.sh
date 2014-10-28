#!/bin/bash

# cpp files to include
echo "src/hash/php_hash_interface.cpp"

# header files to include
echo "hash/php_hash_interface.h"

# declared PHP functions
echo "//"

# init functions
echo "init_interface_HashInterface(TSRMLS_C);"
