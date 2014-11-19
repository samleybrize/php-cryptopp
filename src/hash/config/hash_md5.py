import os

def config(cryptoppDir):
    config = {}

    # check if md5 is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/md5.h"):
        config["enabled"] = False
        return config

    config["enabled"]               = True
    config["srcFileList"]           = ["hash/php_md5.cpp"]
    config["headerFileList"]        = ["hash/php_md5.h"]
    config["phpMinitStatements"]    = ["CRYPTOPP_HASH_INIT_CLASS_FUNC_CALL(HashMd5)"]

    config["hashAssoc"]             = {}
    config["hashAssoc"]["md5"]      = "CryptoPP::Weak::MD5";

    return config
