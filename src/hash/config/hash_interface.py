import os

def config(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["hash/php_hash_interface.cpp"]
    config["headerFileList"]        = ["hash/php_hash_interface.h"]
    config["phpMinitStatements"]    = ["init_interface_HashInterface(TSRMLS_C);"]

    return config
