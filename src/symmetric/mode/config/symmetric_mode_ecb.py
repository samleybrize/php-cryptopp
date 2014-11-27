import os

def getConfig(cryptoppDir):
    config = {}

    # check if modes are supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/modes.h"):
        config["enabled"] = False
        return config

    config["enabled"]                   = True
    config["srcFileList"]               = ["symmetric/mode/php_ecb.cpp"]
    config["headerFileList"]            = ["symmetric/mode/php_ecb.h"]
    config["phpMinitStatements"]        = ["CRYPTOPP_SYMMETRIC_MODE_INIT_CLASS_FUNC_CALL(SymmetricModeEcb)"]
    config["hashCryptoppHeaders"]       = ["modes.h"]

    return config
