import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["symmetric/cipher/authenticated/authenticated_symmetric_cipher_proxy.cpp"]
    config["headerFileList"]        = ["symmetric/cipher/authenticated/authenticated_symmetric_cipher_proxy.h"]

    return config
