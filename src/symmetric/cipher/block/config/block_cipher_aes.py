import os

def getConfig(cryptoppDir):
    config = {}

    # check if aes is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/aes.h"):
        config["enabled"] = False
        return config

    config["enabled"]                   = True
    config["srcFileList"]               = ["symmetric/cipher/block/php_aes.cpp"]
    config["headerFileList"]            = ["symmetric/cipher/block/php_aes.h"]
    config["phpMinitStatements"]        = ["init_class_BlockCipherAes(TSRMLS_C);"]
    config["hashCryptoppHeaders"]       = ["aes.h"]

    return config
