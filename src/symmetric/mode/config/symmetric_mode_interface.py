import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["symmetric/mode/php_symmetric_mode_interface.cpp"]
    config["headerFileList"]        = ["symmetric/mode/php_symmetric_mode_interface.h"]
    config["phpMinitStatements"]    = ["init_interface_SymmetricModeInterface(TSRMLS_C);"]

    return config
