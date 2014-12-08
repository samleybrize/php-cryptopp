import os

def getConfig(cryptoppDir):
    config = {}

    # check if sosemanuk is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/sosemanuk.h"):
        config["enabled"] = False
        return config

    config["enabled"]                   = True
    config["srcFileList"]               = ["symmetric/cipher/stream/php_sosemanuk.cpp"]
    config["headerFileList"]            = ["symmetric/cipher/stream/php_sosemanuk.h"]
    config["phpMinitStatements"]        = ["init_class_StreamCipherSosemanuk(TSRMLS_C);"]

    return config
