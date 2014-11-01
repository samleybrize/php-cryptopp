import argparse
import imp
import os
import re
import sys

EXTENSION_VERSION = "0.1"

def loadModuleFromFile(filepath):
    modname = os.path.splitext(filepath)[0]
    modname = os.path.normpath(modname)
    modname = modname.replace("\\", "/")
    modname = re.sub("[^a-zA-Z0-9/]", "_", modname)
    modname = modname.replace("/", "-")

    module  = imp.load_source(modname, os.path.abspath(filepath))
    return module

# retrieve command line args
parser      = argparse.ArgumentParser()
parser.add_argument("cryptopp_dir", help="Path to the Crypto++ headers")
args        = parser.parse_args()
cryptoppDir = args.cryptopp_dir

# verify that config.m4 exists
if not os.path.isfile("config.m4"):
    print("ERROR: cannot find 'config.m4' file")
    sys.exit(1)

# config file list
configFileList = []
configFileList.append("src/exception/config/exception.py")

configFileList.append("src/hash/config/hash.py")
configFileList.append("src/hash/config/hash_interface.py")
configFileList.append("src/hash/config/hash_md5.py")
configFileList.append("src/hash/config/hash_sha1.py")
configFileList.append("src/hash/config/hash_sha3.py")

configFileList.append("src/prng/config/rbg_interface.py")
configFileList.append("src/prng/config/rbg.py")

# process all config scripts
phpMinitStatements  = []
srcFileList         = ["php_cryptopp.cpp"]
headerFileList      = []

for configFile in configFileList:
    # verify that config file exists
    if not os.path.isfile(configFile):
        print("Config file " + configFile + " does not exists")
        sys.exit(1)

    # retrieve config
    module = loadModuleFromFile(configFile)
    config = module.config(cryptoppDir)

    if False == config["enabled"]:
        continue

    if "srcFileList" in config:
        srcFileList.extend(config["srcFileList"])

    if "headerFileList" in config:
        headerFileList.extend(config["headerFileList"])

    if "phpMinitStatements" in config:
        phpMinitStatements.extend(config["phpMinitStatements"])

# build includes for header file
headerFileIncludes = "";

for i in headerFileList:
    headerFileIncludes += "#include \"" + i + "\"\n"

# create main header file
mainHeaderContent   = open("src/php_cryptopp.raw.h", "r").read()
configureInclusion  = headerFileIncludes + "\n\n#define PHP_MINIT_STATEMENTS " + " ".join(phpMinitStatements)
mainHeaderContent   = mainHeaderContent.replace("//%configure_inclusion%", configureInclusion)
mainHeaderContent   = mainHeaderContent.replace("%ext_version%", EXTENSION_VERSION)

open("src/php_cryptopp.h", "w").write(mainHeaderContent)

# print the list of source files to add
for key, srcFile in enumerate(srcFileList):
    srcFileList[key] = "src/" + srcFile

print(" ".join(srcFileList))
