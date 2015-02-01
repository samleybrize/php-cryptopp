import os

def getConfig(cryptoppDir):
    config = {}

    # check if sosemanuk is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/salsa.h"):
        config["enabled"] = False
        return config

    config["enabled"]                   = True
    config["srcFileList"]               = ["symmetric/cipher/stream/php_xsalsa20.cpp"]
    config["headerFileList"]            = ["symmetric/cipher/stream/php_xsalsa20.h"]
    config["phpMinitStatements"]        = ["init_class_StreamCipherXSalsa20(TSRMLS_C);"]

    return config
