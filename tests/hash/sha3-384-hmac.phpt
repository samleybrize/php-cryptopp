--TEST--
Hash algorithm: HMAC-SHA3-384
--SKIPIF--
<?php
if (!class_exists("Cryptopp\HashSha3_384")) {
    echo "skip sha3 not available";
}
?>
--FILE--
<?php

$hash = new Cryptopp\HashSha3_384();
$hmac = new Cryptopp\MacHmac($hash);

$hmac->setKey(Cryptopp\HexUtils::hex2bin("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b"));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest("Hi There")));

$hmac->setKey("Jefe");
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest("what do ya want for nothing?")));

$hmac->setKey(Cryptopp\HexUtils::hex2bin("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest(Cryptopp\HexUtils::hex2bin(str_repeat("dd", 50)))));

$hmac->setKey(Cryptopp\HexUtils::hex2bin("0102030405060708090a0b0c0d0e0f10111213141516171819"));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest(Cryptopp\HexUtils::hex2bin(str_repeat("cd", 50)))));

$hmac->setKey(Cryptopp\HexUtils::hex2bin("0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c"));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest("Test With Truncation")));

$hmac->setKey(Cryptopp\HexUtils::hex2bin(str_repeat("aa", 140)));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest("Test Using Larger Than Block-Size Key - Hash Key First")));

$hmac->setKey(Cryptopp\HexUtils::hex2bin(str_repeat("aa", 140)));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest("Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data 123456789012345678901234567890123456789012345678901234567890123456")));

?>
--EXPECT--
string(96) "c982142522e2b9e11d8f39a2952f24bab2eb7072900e780ce36e20290e9a3e66f1ff1f38302e7b8cab8a0fc44261d475"
string(96) "4a2074dd970b8eaeed36d0efcab4b5ebc88d47d34d780937093b2efda4fb32b5453d7530795ae547c814ffe1cec770ae"
string(96) "7061b16d0b1e8e361f3bf3cba97341ff2fc7aafa95aa398b73a898db899f85c5888fa191c3add458c1820051095e1df9"
string(96) "54c3f01132ba3779f90b03e58938c897bee52b23647436e3255e6d44efa91d6a70a4fa4f2e71a2a079287cbb23565286"
string(96) "02a75898ef1d53e6bc7647b55ef0b7436576d6c3106d2daf3c10c67ef35b6953b5c4272c2f499a2f7e56e3b3ea8a2af8"
string(96) "0a65039917aa77b75c5472b99abd644cf54b83946e7623b2de27d2e165440a31bcc2952b3eab17cafa429dbe34a2a924"
string(96) "f4cff82f4c59de0007e43f68b36b0dbf72278fb7e192d51dc52bfef44cac9373bfd9367a0c3004c757363cc9d487bfbe"
