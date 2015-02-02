import os

def getConfig(cryptoppDir):
    config = {}

    # check if ccm is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/ccm.h"):
        config["enabled"] = False
        return config

    config["enabled"]                   = True
    config["srcFileList"]               = ["symmetric/cipher/authenticated/php_ccm.cpp"]
    config["headerFileList"]            = ["symmetric/cipher/authenticated/php_ccm.h"]
    config["phpMinitStatements"]        = ["init_class_AuthenticatedSymmetricCipherCcm(TSRMLS_C);"]

    return config
