import os

def getConfig(cryptoppDir):
    config = {}

    # check if sha3 is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/sha3.h"):
        config["enabled"] = False
        return config

    config["enabled"]                       = True
    config["srcFileList"]                   = ["hash/php_sha3.cpp"]
    config["headerFileList"]                = ["hash/php_sha3.h"]
    config["phpMinitStatements"]            = ["init_classes_HashSha3(TSRMLS_C);"]
    config["hashCryptoppHeaders"]           = ["sha3.h"]

    config["hashNativeAssoc"]               = {}
    config["hashNativeAssoc"]["sha3_224"]   = "CryptoPP::SHA3_224";
    config["hashNativeAssoc"]["sha3_256"]   = "CryptoPP::SHA3_256";
    config["hashNativeAssoc"]["sha3_384"]   = "CryptoPP::SHA3_384";
    config["hashNativeAssoc"]["sha3_512"]   = "CryptoPP::SHA3_512";

    return config
