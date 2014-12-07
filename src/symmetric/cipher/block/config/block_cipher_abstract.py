import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["symmetric/cipher/block/php_block_cipher_abstract.cpp"]
    config["headerFileList"]        = ["symmetric/cipher/block/php_block_cipher_abstract.h"]
    config["phpMinitStatements"]    = ["init_class_BlockCipherAbstract(TSRMLS_C);"]

    return config
