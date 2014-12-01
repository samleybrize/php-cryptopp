import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["mac/php_mac_abstract.cpp"]
    config["headerFileList"]        = ["mac/php_mac_abstract.h"]
    config["phpMinitStatements"]    = ["init_class_MacAbstract(TSRMLS_C);"]

    return config
