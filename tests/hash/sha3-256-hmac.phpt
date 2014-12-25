--TEST--
Hash algorithm: HMAC-SHA3-256
--FILE--
<?php

$hash = new Cryptopp\HashSha3_256();
$hmac = new Cryptopp\MacHmac($hash);

$hmac->setKey(hex2bin("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b"));
var_dump(bin2hex($hmac->calculateDigest("Hi There")));

$hmac->setKey("Jefe");
var_dump(bin2hex($hmac->calculateDigest("what do ya want for nothing?")));

$hmac->setKey(hex2bin("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
var_dump(bin2hex($hmac->calculateDigest(hex2bin(str_repeat("dd", 50)))));

$hmac->setKey(hex2bin("0102030405060708090a0b0c0d0e0f10111213141516171819"));
var_dump(bin2hex($hmac->calculateDigest(hex2bin(str_repeat("cd", 50)))));

$hmac->setKey(hex2bin("0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c"));
var_dump(bin2hex($hmac->calculateDigest("Test With Truncation")));

$hmac->setKey(hex2bin(str_repeat("aa", 120)));
var_dump(bin2hex($hmac->calculateDigest("Test Using Larger Than Block-Size Key - Hash Key First")));

$hmac->setKey(hex2bin(str_repeat("aa", 120)));
var_dump(bin2hex($hmac->calculateDigest("Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data 12345678901234567890123456789012345")));

?>
--EXPECTF--
string(64) "a41507de38ad9575f27ed2265b5e146bf51859c6599f0f50cfe1c39e82387b28"
string(64) "e6ba99b63c127a426957a950a96fdd53adc99df3e7e599cd1fbe6a29b0d4de2a"
string(64) "edf5122e336a6e478091bb69e142148a7be1d2457bd287e0b288dd3f3bebffc7"
string(64) "03bf95dd65455f237f787637bbb6c7eba546e4cb2cc1f2f519afb28875aaa2b3"
string(64) "64adb40cd2c2500c653fb3412110ae7d41c4c406f436f9fa4ca73cddf66f6d04"
string(64) "5e625dbe0f0651b1dffb8fb5f99b04551315c6eff22571d0131f6d64393d01cc"
string(64) "df6f118c50c7679a7fea7f40d7618010791a9d0779119f7893bdc0cd9af858ad"
