import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["padding/php_no_padding.cpp"]
    config["headerFileList"]        = ["padding/php_no_padding.h"]
    config["phpMinitStatements"]    = ["init_class_PaddingNoPadding(TSRMLS_C);"]

    return config
