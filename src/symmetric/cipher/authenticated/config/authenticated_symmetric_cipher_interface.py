import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["symmetric/cipher/authenticated/php_authenticated_symmetric_cipher_interface.cpp"]
    config["headerFileList"]        = ["symmetric/cipher/authenticated/php_authenticated_symmetric_cipher_interface.h"]
    config["phpMinitStatements"]    = ["init_interface_AuthenticatedSymmetricCipherInterface(TSRMLS_C);"]

    return config
