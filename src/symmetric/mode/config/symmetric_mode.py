import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["symmetric/mode/php_symmetric_mode.cpp"]
    config["headerFileList"]        = ["symmetric/mode/php_symmetric_mode.h"]
    config["phpMinitStatements"]    = ["init_class_SymmetricMode(TSRMLS_C);"]

    return config
