import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["padding/php_padding_interface.cpp"]
    config["headerFileList"]        = ["padding/php_padding_interface.h"]
    config["phpMinitStatements"]    = ["init_interface_PaddingInterface(TSRMLS_C);"]

    return config
