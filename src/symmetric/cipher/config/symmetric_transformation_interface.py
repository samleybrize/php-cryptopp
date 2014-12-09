import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["symmetric/cipher/php_symmetric_transformation_interface.cpp"]
    config["headerFileList"]        = ["symmetric/cipher/php_symmetric_transformation_interface.h"]
    config["phpMinitStatements"]    = ["init_interface_SymmetricTransformationInterface(TSRMLS_C);"]

    return config
