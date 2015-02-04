--TEST--
Padding: PKCS #7 (errors)
--FILE--
<?php

$padding = new Cryptopp\PaddingPkcs7();

// test errors (pad)
echo "- errors (pad):\n";
try {
    $padding->pad("", 0);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $padding->pad("", 257);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// test errors (unpad)
echo "- errors (unpad):\n";
try {
    $padding->unpad("", 0);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $padding->unpad("", 257);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $padding->unpad("123", 4);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $padding->unpad("1234", 4);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $padding->unpad(Cryptopp\HexUtils::hex2bin("04040304"), 4);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// bad arguments
echo "- bad arguments:\n";
var_dump(@$padding->pad(array(), 4));
echo "$php_errormsg\n";
var_dump(@$padding->pad("azerty", array()));
echo "$php_errormsg\n";
var_dump(@$padding->unpad(array(), 4));
echo "$php_errormsg\n";
var_dump(@$padding->unpad("azerty", array()));
echo "$php_errormsg\n";

?>
--EXPECT--
- errors (pad):
Cryptopp\PaddingPkcs7 : block size cannot be lower than 1, 0 given
Cryptopp\PaddingPkcs7 : PKCS #7 padding does not handle block sizes higher than 256
- errors (unpad):
Cryptopp\PaddingPkcs7 : block size cannot be lower than 1, 0 given
Cryptopp\PaddingPkcs7 : PKCS #7 padding does not handle block sizes higher than 256
Cryptopp\PaddingPkcs7 : data length is not a multiple of block size (block size is 4, data size is 3)
Cryptopp\PaddingPkcs7 : invalid PKCS #7 block padding found
Cryptopp\PaddingPkcs7 : invalid PKCS #7 block padding found
- bad arguments:
bool(false)
Cryptopp\PaddingPkcs7::pad() expects parameter 1 to be string, array given
bool(false)
Cryptopp\PaddingPkcs7::pad() expects parameter 2 to be long, array given
bool(false)
Cryptopp\PaddingPkcs7::unpad() expects parameter 1 to be string, array given
bool(false)
Cryptopp\PaddingPkcs7::unpad() expects parameter 2 to be long, array given
