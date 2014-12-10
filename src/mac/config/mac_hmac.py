import os

def getConfig(cryptoppDir):
    config = {}

    # check if hmac is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/hmac.h"):
        config["enabled"] = False
        return config

    config["enabled"]               = True
    config["srcFileList"]           = ["mac/php_hmac.cpp"]
    config["headerFileList"]        = ["mac/php_hmac.h"]
    config["phpMinitStatements"]    = ["init_class_MacHmac(TSRMLS_C);"]

    return config
