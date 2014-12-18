import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["symmetric/cipher/authenticated/php_authenticated_symmetric_cipher.cpp"]
    config["headerFileList"]        = ["symmetric/cipher/authenticated/php_authenticated_symmetric_cipher.h"]
    config["phpMinitStatements"]    = ["init_class_AuthenticatedSymmetricCipher(TSRMLS_C);"]

    return config
