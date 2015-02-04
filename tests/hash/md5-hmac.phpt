--TEST--
Hash algorithm: HMAC-MD5
--FILE--
<?php

$hash = new Cryptopp\HashMd5();
$hmac = new Cryptopp\MacHmac($hash);

$hmac->setKey(Cryptopp\HexUtils::hex2bin("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b"));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest("Hi There")));

$hmac->setKey("Jefe");
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest("what do ya want for nothing?")));

$hmac->setKey(Cryptopp\HexUtils::hex2bin("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest(Cryptopp\HexUtils::hex2bin(str_repeat("dd", 50)))));

$hmac->setKey(Cryptopp\HexUtils::hex2bin("0102030405060708090a0b0c0d0e0f10111213141516171819"));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest(Cryptopp\HexUtils::hex2bin(str_repeat("cd", 50)))));

$hmac->setKey(Cryptopp\HexUtils::hex2bin("0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c"));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest("Test With Truncation")));

$hmac->setKey(Cryptopp\HexUtils::hex2bin(str_repeat("aa", 80)));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest("Test Using Larger Than Block-Size Key - Hash Key First")));

$hmac->setKey(Cryptopp\HexUtils::hex2bin(str_repeat("aa", 80)));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest("Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data")));

?>
--EXPECT--
string(32) "9294727a3638bb1c13f48ef8158bfc9d"
string(32) "750c783e6ab0b503eaa86e310a5db738"
string(32) "56be34521d144c88dbb8c733f0e8b3f6"
string(32) "697eaf0aca3a3aea3a75164746ffaa79"
string(32) "56461ef2342edc00f9bab995690efd4c"
string(32) "6b1ab7fe4bd7bf8f0b62e6ce61b9d0cd"
string(32) "6f630fad67cda0ee1fb1f562db3aa53e"
