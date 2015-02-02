import os

def getConfig(cryptoppDir):
    config = {}

    # check if panama is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/panama.h"):
        config["enabled"] = False
        return config

    config["enabled"]                   = True
    config["srcFileList"]               = ["symmetric/cipher/stream/php_panama.cpp"]
    config["headerFileList"]            = ["symmetric/cipher/stream/php_panama.h"]
    config["phpMinitStatements"]        = ["init_class_StreamCipherPanama(TSRMLS_C);"]

    return config
