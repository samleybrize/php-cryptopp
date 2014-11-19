import os

def config(cryptoppDir):
    config = {}

    # check if ttmac is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/ttmac.h"):
        config["enabled"] = False
        return config

    config["enabled"]               = True
    config["srcFileList"]           = ["mac/php_ttmac.cpp"]
    config["headerFileList"]        = ["mac/php_ttmac.h"]
    config["phpMinitStatements"]    = ["CRYPTOPP_MAC_INIT_CLASS_FUNC_CALL(MacTwoTrackMac)"]

    return config
