import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["symmetric/mode/php_symmetric_mode_abstract.cpp"]
    config["headerFileList"]        = ["symmetric/mode/php_symmetric_mode_abstract.h"]
    config["phpMinitStatements"]    = ["init_class_SymmetricModeAbstract(TSRMLS_C);"]

    return config
