import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["hash/php_hash_abstract.cpp"]
    config["headerFileList"]        = ["hash/php_hash_abstract.h"]
    config["phpMinitStatements"]    = ["init_class_HashAbstract(TSRMLS_C);"]

    return config
