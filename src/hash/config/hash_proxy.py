import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["hash/hash_proxy.cpp"]
    config["headerFileList"]        = ["hash/hash_proxy.h"]

    return config
