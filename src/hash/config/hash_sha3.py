import os

def config(cryptoppDir):
    config = {}

    # check if sha3 is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/sha3.h"):
        config["enabled"] = False
        return config

    config["enabled"]               = True
    config["srcFileList"]           = ["hash/php_sha3.cpp"]
    config["headerFileList"]        = ["hash/php_sha3.h"]
    config["phpMinitStatements"]    = ["init_classes_HashSha3(TSRMLS_C);"]

    return config
