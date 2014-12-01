import os

def getConfig(cryptoppDir):
    config = {}

    # check if md5 is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/md5.h"):
        config["enabled"] = False
        return config

    config["enabled"]                   = True
    config["srcFileList"]               = ["hash/php_md5.cpp"]
    config["headerFileList"]            = ["hash/php_md5.h"]
    config["phpMinitStatements"]        = ["init_class_HashMd5(TSRMLS_C);"]
    config["hashCryptoppHeaders"]       = ["md5.h"]

    config["hashNativeAssoc"]           = {}
    config["hashNativeAssoc"]["md5"]    = "CryptoPP::Weak::MD5";

    return config
