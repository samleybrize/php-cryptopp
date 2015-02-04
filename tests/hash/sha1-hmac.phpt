--TEST--
Hash algorithm: HMAC-SHA1
--FILE--
<?php

$hash = new Cryptopp\HashSha1();
$hmac = new Cryptopp\MacHmac($hash);

$hmac->setKey(Cryptopp\HexUtils::hex2bin("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b"));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest("Hi There")));

$hmac->setKey("Jefe");
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest("what do ya want for nothing?")));

$hmac->setKey(Cryptopp\HexUtils::hex2bin("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest(Cryptopp\HexUtils::hex2bin(str_repeat("dd", 50)))));

$hmac->setKey(Cryptopp\HexUtils::hex2bin("0102030405060708090a0b0c0d0e0f10111213141516171819"));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest(Cryptopp\HexUtils::hex2bin(str_repeat("cd", 50)))));

$hmac->setKey(Cryptopp\HexUtils::hex2bin("0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c"));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest("Test With Truncation")));

$hmac->setKey(Cryptopp\HexUtils::hex2bin(str_repeat("aa", 80)));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest("Test Using Larger Than Block-Size Key - Hash Key First")));

$hmac->setKey(Cryptopp\HexUtils::hex2bin(str_repeat("aa", 80)));
var_dump(Cryptopp\HexUtils::bin2hex($hmac->calculateDigest("Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data")));

?>
--EXPECT--
string(40) "b617318655057264e28bc0b6fb378c8ef146be00"
string(40) "effcdf6ae5eb2fa2d27416d5f184df9c259a7c79"
string(40) "125d7342b9ac11cd91a39af48aa17b4f63f175d3"
string(40) "4c9007f4026250c6bc8414f9bf50c86c2d7235da"
string(40) "4c1a03424b55e07fe7f27be1d58bb9324a9a5a04"
string(40) "aa4ae5e15272d00e95705637ce8a3b55ed402112"
string(40) "e8e99d0f45237d786d6bbaa7965c7808bbff1a91"
