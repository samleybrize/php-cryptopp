import os

def getConfig(cryptoppDir):
    config = {}

    # check if hmac is supported by installed crypto++
    if not os.path.isfile(cryptoppDir + "/hmac.h"):
        config["enabled"] = False
        return config

    config["enabled"]               = True
    config["srcFileList"]           = ["mac/php_hmac.cpp"]
    config["headerFileList"]        = ["mac/php_hmac.h"]
    config["phpMinitStatements"]    = ["init_class_MacHmac(TSRMLS_C);"]

    return config

def configure(hashNativeAssoc, hashCryptoppHeaders):
    # generate conditional creation
    condition = ""

    for algoName in hashNativeAssoc:
        if not "" == condition:
            condition += " else "

        condition += 'if (0 == algoVarName.compare("' + algoName + '")) { \\\n'
        condition += "        hmacPtrName = new CryptoPP::HMAC<" + hashNativeAssoc[algoName] + ">(); \\\n"
        condition += "    }"

    # generate native header inclusions
    headerInclusions = ""

    for nativeHeaderFile in hashCryptoppHeaders:
        headerInclusions += "#include <" + nativeHeaderFile + ">\n"

    # create hmac dynamic header file
    rawHeaderFile   = os.path.dirname(os.path.dirname(__file__)) + "/php_hmac_d.raw.h"
    rawHeaderFile   = os.path.normpath(rawHeaderFile)
    headerFile      = os.path.dirname(rawHeaderFile) + "/php_hmac_d.h"
    headerFile      = os.path.normpath(headerFile)
    headerContent   = open(rawHeaderFile, "r").read()
    headerContent   = headerContent.replace("%condition%", condition)
    headerContent   = headerContent.replace("//%inclusions%", headerInclusions)
    open(headerFile, "w").write(headerContent)
