import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["padding/php_pkcs7.cpp"]
    config["headerFileList"]        = ["padding/php_pkcs7.h"]
    config["phpMinitStatements"]    = ["init_class_PaddingPkcs7(TSRMLS_C);"]

    return config
