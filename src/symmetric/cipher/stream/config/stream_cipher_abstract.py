import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["symmetric/cipher/stream/php_stream_cipher_abstract.cpp"]
    config["headerFileList"]        = ["symmetric/cipher/stream/php_stream_cipher_abstract.h"]
    config["phpMinitStatements"]    = ["init_class_StreamCipherAbstract(TSRMLS_C);"]

    return config
