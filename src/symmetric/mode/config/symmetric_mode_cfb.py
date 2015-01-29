import os

def getConfig(cryptoppDir):
    config = {}

    # check if modes are supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/modes.h"):
        config["enabled"] = False
        return config

    config["enabled"]                   = True
    config["srcFileList"]               = ["symmetric/mode/php_cfb.cpp"]
    config["headerFileList"]            = ["symmetric/mode/php_cfb.h"]
    config["phpMinitStatements"]        = ["init_class_SymmetricModeCfb(TSRMLS_C);"]

    return config
