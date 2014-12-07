import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["symmetric/cipher/php_block_cipher.cpp"]
    config["headerFileList"]        = ["symmetric/cipher/php_block_cipher.h"]
    config["phpMinitStatements"]    = ["init_class_BlockCipher(TSRMLS_C);"]

    return config
