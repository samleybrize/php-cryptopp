import os

def getConfig(cryptoppDir):
    config = {}

    # check if hmac is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/cmac.h"):
        config["enabled"] = False
        return config

    config["enabled"]               = True
    config["srcFileList"]           = ["mac/php_cmac.cpp"]
    config["headerFileList"]        = ["mac/php_cmac.h"]
    config["phpMinitStatements"]    = ["init_class_MacCmac(TSRMLS_C);"]

    return config
