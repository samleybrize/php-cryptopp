import os

def getConfig(cryptoppDir):
    config = {}

    # check if modes are supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/modes.h"):
        config["enabled"] = False
        return config

    config["enabled"]                   = True
    config["srcFileList"]               = ["symmetric/mode/php_ecb.cpp"]
    config["headerFileList"]            = ["symmetric/mode/php_ecb.h"]
    config["phpMinitStatements"]        = ["init_class_SymmetricModeEcb(TSRMLS_C);"]
    config["hashCryptoppHeaders"]       = ["modes.h"]

    return config
