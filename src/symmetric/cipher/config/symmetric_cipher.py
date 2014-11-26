import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["symmetric/cipher/php_symmetric_cipher.cpp"]
    config["headerFileList"]        = ["symmetric/cipher/php_symmetric_cipher.h"]
    config["phpMinitStatements"]    = ["init_class_SymmetricCipher(TSRMLS_C);"]

    return config
