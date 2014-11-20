import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["exception/php_exception.cpp"]
    config["headerFileList"]        = ["exception/php_exception.h"]
    config["phpMinitStatements"]    = ["init_class_CryptoppException(TSRMLS_C);"]

    return config
