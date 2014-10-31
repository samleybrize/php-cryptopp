import os

def config(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["hash/php_hash.cpp"]
    config["headerFileList"]        = ["hash/php_hash.h"]
    config["phpMinitStatements"]    = ["init_class_Hash(TSRMLS_C);"]

    return config
