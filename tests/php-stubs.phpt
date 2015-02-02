--TEST--
PHP Stubs
--FILE--
<?php

require_once __DIR__ . "/includes/DefinitionNamespace.inc";

function importStubs(array $fileList, $dir) {
    $all = "";

    foreach ($fileList as $file) {
        $filepath = "$dir/$file";

        if (!is_file($filepath)) {
            throw new InvalidArgumentException("Path '$filepath' is not a regular file");
        } elseif (!is_readable($filepath)) {
            throw new InvalidArgumentException("File '$filepath' is not readable");
        }

        // import
        $content = file_get_contents($filepath);
        $content = str_replace("<?php", "", $content);
        $content = str_replace("namespace Cryptopp;", "namespace _Cryptopp;", $content);
        $content = str_replace("Cryptopp\\", "_Cryptopp\\", $content);

        if (false === strpos($content, "namespace _Cryptopp;")) {
            continue;
        }

        // special cases
        if ("Symmetric/Cipher/Authenticated/AuthenticatedSymmetricCipherGeneric.php" == $file) {
            // function is here only to indicates a slightly different behavior
            $content = preg_replace("#final public function setKey.*\n#U", "", $content);
        }

        $all .= $content;
    }

    return $all;
}

function fixStubDefinitions(array &$definitions) {
    foreach ($definitions as $k => &$definition) {
        // fix key
        if (is_string($k) && false !== strpos($k, "_Cryptopp")) {
            $k2                 = str_replace("_Cryptopp", "Cryptopp", $k);
            $definitions[$k2]   = $definition;
            unset($definitions[$k]);
        }

        // fix value
        if (is_array($definition)) {
            fixStubDefinitions($definition);
            continue;
        } elseif (!is_string($definition) || false === strpos($definition, "_Cryptopp")) {
            continue;
        }

        $definition = str_replace("_Cryptopp", "Cryptopp", $definition);
    }
}

function arrayRecursiveDiff($aArray1, $aArray2) {
    $aReturn = array();

    foreach ($aArray1 as $mKey => $mValue) {
        if (array_key_exists($mKey, $aArray2)) {
            if (is_array($mValue)) {
                $aRecursiveDiff = arrayRecursiveDiff($mValue, $aArray2[$mKey]);

                if (count($aRecursiveDiff)) {
                    $aReturn[$mKey] = $aRecursiveDiff;
                }
            } elseif ($mValue !== $aArray2[$mKey]) {
                $aReturn[$mKey] = $mValue;
            }
        } else {
            $aReturn[$mKey] = $mValue;
        }
    }

    return $aReturn;
}

$fileList = array(
    "Exception/CryptoppException.php",
    "Exception/CryptoppMacVerificationFailedException.php",
    "Filter/SymmetricTransformationFilter.php",
    "Filter/AuthenticatedSymmetricTransformationFilter.php",
    "Filter/HashTransformationFilter.php",
    "RandomByteGenerator/RandomByteGeneratorInterface.php",
    "RandomByteGenerator/RandomByteGenerator.php",
    "Hash/HashTransformationInterface.php",
    "Hash/HashInterface.php",
    "Hash/HashAbstract.php",
    "Hash/Hash.php",
    "Hash/HashSha3_384.php",
    "Hash/HashMd5.php",
    "Hash/HashSha3_512.php",
    "Hash/HashSha1.php",
    "Hash/HashSha3_224.php",
    "Hash/HashSha3_256.php",
    "Symmetric/Cipher/SymmetricCipherInterface.php",
    "Symmetric/Cipher/SymmetricTransformationInterface.php",
    "Symmetric/Cipher/Stream/StreamCipherInterface.php",
    "Symmetric/Cipher/Stream/StreamCipherAbstract.php",
    "Symmetric/Cipher/Stream/StreamCipherPanama.php",
    "Symmetric/Cipher/Stream/StreamCipherSalsa20.php",
    "Symmetric/Cipher/Stream/StreamCipherSosemanuk.php",
    "Symmetric/Cipher/Stream/StreamCipherXSalsa20.php",
    "Symmetric/Cipher/Stream/StreamCipher.php",
    "Symmetric/Cipher/Block/BlockCipherInterface.php",
    "Symmetric/Cipher/Block/BlockCipherAbstract.php",
    "Symmetric/Cipher/Block/BlockCipherAes.php",
    "Symmetric/Cipher/Block/BlockCipher.php",
    "Symmetric/Cipher/Authenticated/AuthenticatedSymmetricCipherInterface.php",
    "Symmetric/Cipher/Authenticated/AuthenticatedSymmetricCipherAbstract.php",
    "Symmetric/Cipher/Authenticated/AuthenticatedSymmetricCipherGeneric.php",
    "Symmetric/Cipher/Authenticated/AuthenticatedSymmetricCipherGcm.php",
    "Symmetric/Cipher/Authenticated/AuthenticatedSymmetricCipherCcm.php",
    "Symmetric/Cipher/Authenticated/AuthenticatedSymmetricCipherEax.php",
    "Symmetric/Cipher/Authenticated/AuthenticatedSymmetricCipher.php",
    "Symmetric/Mode/SymmetricModeInterface.php",
    "Symmetric/Mode/SymmetricModeAbstract.php",
    "Symmetric/Mode/SymmetricModeCbc.php",
    "Symmetric/Mode/SymmetricModeCfb.php",
    "Symmetric/Mode/SymmetricModeCtr.php",
    "Symmetric/Mode/SymmetricModeEcb.php",
    "Symmetric/Mode/SymmetricModeOfb.php",
    "Symmetric/Mode/SymmetricMode.php",
    "Padding/PaddingInterface.php",
    "Padding/PaddingNoPadding.php",
    "Padding/PaddingPkcs7.php",
    "Mac/MacInterface.php",
    "Mac/MacAbstract.php",
    "Mac/MacTwoTrackMac.php",
    "Mac/Mac.php",
    "Mac/MacHmac.php",
    "constants.php",
);

$dir = __DIR__ . "/../php-stubs";
$all = importStubs($fileList, $dir);
eval($all);

$definitionInternal = new DefinitionNamespace("Cryptopp");
$definitionInternal = $definitionInternal->getDefinition();
$definitionStubs    = new DefinitionNamespace("_Cryptopp");
$definitionStubs    = $definitionStubs->getDefinition();
fixStubDefinitions($definitionStubs);

var_dump(arrayRecursiveDiff($definitionInternal, $definitionStubs));

?>
--EXPECT--
array(0) {
}
