import os

def config(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["exception/php_exception.cpp"]
    config["headerFileList"]        = ["exception/php_exception.h"]
    config["phpMinitStatements"]    = ["init_class_CryptoPPException(TSRMLS_C);"]

    return config
