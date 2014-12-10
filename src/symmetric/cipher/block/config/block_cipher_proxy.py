import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["symmetric/cipher/block/block_cipher_proxy.cpp"]
    config["headerFileList"]        = ["symmetric/cipher/block/block_cipher_proxy.h"]

    return config
