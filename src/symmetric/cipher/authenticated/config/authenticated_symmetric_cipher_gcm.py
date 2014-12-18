import os

def getConfig(cryptoppDir):
    config = {}

    # check if sosemanuk is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/gcm.h"):
        config["enabled"] = False
        return config

    config["enabled"]                   = True
    config["srcFileList"]               = ["symmetric/cipher/authenticated/php_gcm.cpp"]
    config["headerFileList"]            = ["symmetric/cipher/authenticated/php_gcm.h"]
    config["phpMinitStatements"]        = ["init_class_AuthenticatedSymmetricCipherGcm(TSRMLS_C);"]

    return config
