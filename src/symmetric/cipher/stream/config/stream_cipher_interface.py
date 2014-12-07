import os

def getConfig(cryptoppDir):
    config = {}

    config["enabled"]               = True
    config["srcFileList"]           = ["symmetric/cipher/stream/php_stream_cipher_interface.cpp"]
    config["headerFileList"]        = ["symmetric/cipher/stream/php_stream_cipher_interface.h"]
    config["phpMinitStatements"]    = ["init_interface_StreamCipherInterface(TSRMLS_C);"]

    return config
