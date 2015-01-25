import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]                   = True
    config["srcFileList"]               = ["utils/php_hex_bin.cpp"]
    config["headerFileList"]            = ["utils/php_hex_bin.h"]
    config["functionDeclarationList"]   = ["PHP_HEX_BIN_DECLARATION"]

    return config
