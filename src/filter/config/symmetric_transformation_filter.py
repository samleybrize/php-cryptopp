import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["filter/php_symmetric_transformation_filter.cpp"]
    config["headerFileList"]        = ["filter/php_symmetric_transformation_filter.h"]
    config["phpMinitStatements"]    = ["init_class_SymmetricTransformationFilter(TSRMLS_C);"]

    return config
