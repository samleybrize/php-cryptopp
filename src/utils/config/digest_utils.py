import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["utils/php_digest_utils.cpp"]
    config["headerFileList"]        = ["utils/php_digest_utils.h"]
    config["phpMinitStatements"]    = ["init_class_DigestUtils(TSRMLS_C);"]

    return config
