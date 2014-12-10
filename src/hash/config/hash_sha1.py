import os

def getConfig(cryptoppDir):
    config = {}

    # check if sha1 is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/sha.h"):
        config["enabled"] = False
        return config

    config["enabled"]                   = True
    config["srcFileList"]               = ["hash/php_sha1.cpp"]
    config["headerFileList"]            = ["hash/php_sha1.h"]
    config["phpMinitStatements"]        = ["init_class_HashSha1(TSRMLS_C);"]

    return config
