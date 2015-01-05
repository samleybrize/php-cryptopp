import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["mac/mac_proxy.cpp"]
    config["headerFileList"]        = ["mac/mac_proxy.h"]

    return config
