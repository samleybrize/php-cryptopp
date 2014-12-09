import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["symmetric/cipher/symmetric_transformation_proxy.cpp"]
    config["headerFileList"]        = ["symmetric/cipher/symmetric_transformation_proxy.h"]

    return config
