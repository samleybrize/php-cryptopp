import os

def config(cryptoppDir):
    config = {}

    # check if sha1 is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/sha.h"):
        config["enabled"] = False
        return config

    config["enabled"]               = True
    config["srcFileList"]           = ["hash/php_sha1.cpp"]
    config["headerFileList"]        = ["hash/php_sha1.h"]
    config["phpMinitStatements"]    = ["CRYPTOPP_HASH_INIT_CLASS_FUNC_CALL(HashSha1)"]

    config["hashAssoc"]             = {}
    config["hashAssoc"]["sha1"]     = "CryptoPP::SHA1";

    return config
