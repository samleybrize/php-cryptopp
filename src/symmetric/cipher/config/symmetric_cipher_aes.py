import os

def getConfig(cryptoppDir):
    config = {}

    # check if aes is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/aes.h"):
        config["enabled"] = False
        return config

    config["enabled"]                   = True
    config["srcFileList"]               = ["symmetric/cipher/php_aes.cpp"]
    config["headerFileList"]            = ["symmetric/cipher/php_aes.h"]
    config["phpMinitStatements"]        = ["init_class_SymmetricCipherAes(TSRMLS_C);"]
    config["hashCryptoppHeaders"]       = ["aes.h"]

    return config
