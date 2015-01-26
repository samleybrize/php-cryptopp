import os

def getConfig(cryptoppDir):
    config = {}

    # check if gcm is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/eax.h"):
        config["enabled"] = False
        return config

    config["enabled"]                   = True
    config["srcFileList"]               = ["symmetric/cipher/authenticated/php_eax.cpp"]
    config["headerFileList"]            = ["symmetric/cipher/authenticated/php_eax.h"]
    config["phpMinitStatements"]        = ["init_class_AuthenticatedSymmetricCipherEax(TSRMLS_C);"]

    return config
