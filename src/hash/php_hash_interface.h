#ifndef HASH_HASH_INTERFACE_H
#define HASH_HASH_INTERFACE_H

extern "C" {
#include "php.h"
}

extern zend_class_entry *cryptopp_test_ce_hash_interface;

void init_interface_HashInterface(TSRMLS_D);

#endif

