import os

def getConfig(cryptoppDir):
    config = {}

    # check if md5 is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/aes.h"):
        config["enabled"] = False
        return config

    config["enabled"]                   = True
    config["srcFileList"]               = ["symmetric/cipher/php_aes.cpp"]
    config["headerFileList"]            = ["symmetric/cipher/php_aes.h"]
    config["phpMinitStatements"]        = ["CRYPTOPP_SYMMETRIC_CIPHER_INIT_CLASS_FUNC_CALL(SymmetricCipherAes)"]
    config["hashCryptoppHeaders"]       = ["aes.h"]

    return config
