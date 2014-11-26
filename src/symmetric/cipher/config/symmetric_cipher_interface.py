import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["symmetric/cipher/php_symmetric_cipher_interface.cpp"]
    config["headerFileList"]        = ["symmetric/cipher/php_symmetric_cipher_interface.h"]
    config["phpMinitStatements"]    = ["init_interface_SymmetricCipherInterface(TSRMLS_C);"]

    return config
