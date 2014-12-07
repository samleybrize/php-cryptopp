import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["symmetric/cipher/stream/php_stream_cipher.cpp"]
    config["headerFileList"]        = ["symmetric/cipher/stream/php_stream_cipher.h"]
    config["phpMinitStatements"]    = ["init_class_StreamCipher(TSRMLS_C);"]

    return config
