import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]                   = True
    config["srcFileList"]               = ["utils/php_hash_equals.cpp"]
    config["headerFileList"]            = ["utils/php_hash_equals.h"]
    config["functionDeclarationList"]   = ["PHP_HASH_EQUALS_DECLARATION"]

    return config
