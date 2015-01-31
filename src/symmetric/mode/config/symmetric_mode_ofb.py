import os

def getConfig(cryptoppDir):
    config = {}

    # check if modes are supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/modes.h"):
        config["enabled"] = False
        return config

    config["enabled"]                   = True
    config["srcFileList"]               = ["symmetric/mode/php_ofb.cpp"]
    config["headerFileList"]            = ["symmetric/mode/php_ofb.h"]
    config["phpMinitStatements"]        = ["init_class_SymmetricModeOfb(TSRMLS_C);"]

    return config
