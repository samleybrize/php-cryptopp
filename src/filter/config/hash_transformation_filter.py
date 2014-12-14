import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["filter/php_hash_transformation_filter.cpp"]
    config["headerFileList"]        = ["filter/php_hash_transformation_filter.h"]
    config["phpMinitStatements"]    = ["init_class_HashTransformationFilter(TSRMLS_C);"]

    return config
