import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["hash/php_hash_transformation_interface.cpp"]
    config["headerFileList"]        = ["hash/php_hash_transformation_interface.h"]
    config["phpMinitStatements"]    = ["init_interface_HashTransformationInterface(TSRMLS_C);"]

    return config
