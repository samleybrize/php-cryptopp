import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["mac/php_mac.cpp"]
    config["headerFileList"]        = ["mac/php_mac.h"]
    config["phpMinitStatements"]    = ["init_class_Mac(TSRMLS_C);"]

    return config
