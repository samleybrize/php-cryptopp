import argparse
import imp
import os
import re
import sys

def loadModuleFromFile(filepath):
    modname = os.path.normpath(filepath)
    modname = filepath.replace("\\", "/")
    modname = re.sub("[^a-zA-Z0-9/]", "_", modname)
    modname = filepath.replace("/", ".")

    module  = imp.load_source(modname, filepath)
    return module

# retrieve command line args
parser      = argparse.ArgumentParser()
parser.add_argument("cryptopp_dir", help="Path to the Crypto++ headers")
args        = parser.parse_args()
cryptoppDir = args.cryptopp_dir

# TODO test if config.m4 exists

# config file list
configFileList = []
configFileList.append("src/hash/config/hash.py")
configFileList.append("src/hash/config/hash_interface.py")
configFileList.append("src/hash/config/hash_md5.py")
configFileList.append("src/hash/config/hash_sha1.py")
configFileList.append("src/hash/config/hashsha3.py")

# TODO comment
phpMinitStatements  = []
srcFileList         = []
headerFileList      = []

for configFile in configFileList:
    # verify that file exists
    if not os.path.isfile(configFile):
        print("Config file " + configFile + " does not exists")
        sys.exit(1)

    # TODO
    module = loadModuleFromFile(configFile)
    config = module.config(cryptoppDir)

    if False == config["enabled"]:
        continue

    # TODO test array keys
    srcFileList.extend(config["srcFileList"])
    phpMinitStatements.extend(config["phpMinitStatements"])
    headerFileList.extend(config["headerFileList"])

# build includes for header file
headerFileIncludes = "";

for i in headerFileList:
    headerFileIncludes += "#include \"" + i + "\"\n"

# TODO write PHP function list to a header file

# TODO print the list of source files to add
