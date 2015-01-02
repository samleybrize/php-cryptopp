import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]                   = True
    config["srcFileList"]               = ["symmetric/cipher/authenticated/php_authenticated_symmetric_cipher_generic.cpp"]
    config["headerFileList"]            = ["symmetric/cipher/authenticated/php_authenticated_symmetric_cipher_generic.h"]
    config["phpMinitStatements"]        = ["init_class_AuthenticatedSymmetricCipherGeneric(TSRMLS_C);"]

    return config
