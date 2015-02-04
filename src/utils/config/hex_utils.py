import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["utils/php_hex_utils.cpp"]
    config["headerFileList"]        = ["utils/php_hex_utils.h"]
    config["phpMinitStatements"]    = ["init_class_HexUtils(TSRMLS_C);"]

    return config
