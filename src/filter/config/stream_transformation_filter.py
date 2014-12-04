import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["filter/php_stream_transformation_filter.cpp"]
    config["headerFileList"]        = ["filter/php_stream_transformation_filter.h"]
    config["phpMinitStatements"]    = ["init_class_StreamTransformationFilter(TSRMLS_C);"]

    return config
