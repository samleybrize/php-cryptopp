import os

def config(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["prng/php_rbg_interface.cpp"]
    config["headerFileList"]        = ["prng/php_rbg_interface.h"]
    config["phpMinitStatements"]    = ["init_interface_RandomByteGeneratorInterface(TSRMLS_C);"]

    return config
