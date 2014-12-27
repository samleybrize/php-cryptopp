import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["exception/php_mac_verification_failed_exception.cpp"]
    config["headerFileList"]        = ["exception/php_mac_verification_failed_exception.h"]
    config["phpMinitStatements"]    = ["init_class_CryptoppMacVerificationFailedException(TSRMLS_C);"]

    return config
