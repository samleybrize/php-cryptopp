import os

def config(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["mac/php_mac_interface.cpp"]
    config["headerFileList"]        = ["mac/php_mac_interface.h"]
    config["phpMinitStatements"]    = ["init_interface_MacInterface(TSRMLS_C);"]

    return config
