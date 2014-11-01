import os

def config(cryptoppDir):
    config = {}

    # check if AutoSeededRandomPool is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/osrng.h"):
        config["enabled"] = False
        return config

    config["enabled"]               = True
    config["srcFileList"]           = ["prng/php_rbg.cpp"]
    config["headerFileList"]        = ["prng/php_rbg.h"]
    config["phpMinitStatements"]    = ["CRYPTOPP_RBG_INIT_CLASS_FUNC_CALL(RandomByteGenerator)"]

    return config
