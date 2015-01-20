--TEST--
Hash algorithm: HMAC-SHA3-224
--SKIPIF--
<?php
if (!class_exists("Cryptopp\HashSha3_224")) {
    echo "skip sha3 not available";
}
?>
--FILE--
<?php

$hash = new Cryptopp\HashSha3_224();
$hmac = new Cryptopp\MacHmac($hash);

$hmac->setKey(hex2bin("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b"));
var_dump(bin2hex($hmac->calculateDigest("Hi There")));

$hmac->setKey("Jefe");
var_dump(bin2hex($hmac->calculateDigest("what do ya want for nothing?")));

$hmac->setKey(hex2bin("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
var_dump(bin2hex($hmac->calculateDigest(hex2bin(str_repeat("dd", 50)))));

$hmac->setKey(hex2bin("0102030405060708090a0b0c0d0e0f10111213141516171819"));
var_dump(bin2hex($hmac->calculateDigest(hex2bin(str_repeat("cd", 50)))));

$hmac->setKey(hex2bin("0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c"));
var_dump(bin2hex($hmac->calculateDigest("Test With Truncation")));

$hmac->setKey(hex2bin(str_repeat("aa", 80)));
var_dump(bin2hex($hmac->calculateDigest("Test Using Larger Than Block-Size Key - Hash Key First")));

$hmac->setKey(hex2bin(str_repeat("aa", 80)));
var_dump(bin2hex($hmac->calculateDigest("Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data")));

?>
--EXPECT--
string(56) "8eb1677db75287199db38537d4eaec2b983fab8a3274377bccbaef41"
string(56) "99646baa9ef8bcc40ea5fae13f46b593f1bfcbae4ed5db290e1a477a"
string(56) "a17902cfb787acc0eebe298f79e8547d4a6af9378642ff0d6f8685f9"
string(56) "bed764b4d5f7f9ed1bd5aa556aa3207eb40e659001353bf84f67cfc6"
string(56) "30323bc012684bd4fb4e834f04e89412b3ee2f5953d74665afe94a73"
string(56) "3c1c6adace78f796b010c4c9254011641b44ed8630c7d4412445189c"
string(56) "489221a561f669a8f3f672b611b3154498756abf53db7c96ac3cfa95"
