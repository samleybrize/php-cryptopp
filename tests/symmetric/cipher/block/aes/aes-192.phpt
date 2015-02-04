--TEST--
Block cipher: AES-192
--FILE--
<?php

$o = new Cryptopp\BlockCipherAes();

// key length check
echo "- key length check:\n";
var_dump($o->isValidKeyLength(24));
var_dump($o->isValidKeyLength(23));
var_dump($o->isValidKeyLength(25));

// set key
echo "- set key:\n";
$o->setKey("azertyuiopqsdfghjklmwxcv");
var_dump($o->getKey());

// encryptBlock
echo "- encrypt block:\n";
$o->setKey(Cryptopp\HexUtils::hex2bin("8e73b0f7da0e6452c810f32b809079e562f8ead2522c6b7b"));
var_dump(Cryptopp\HexUtils::bin2hex($o->encryptBlock(Cryptopp\HexUtils::hex2bin("6bc1bee22e409f96e93d7e117393172a"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->encryptBlock(Cryptopp\HexUtils::hex2bin("ae2d8a571e03ac9c9eb76fac45af8e51"))));

// decryptBlock
echo "- decrypt block:\n";
var_dump(Cryptopp\HexUtils::bin2hex($o->decryptBlock(Cryptopp\HexUtils::hex2bin("bd334f1d6e45f25ff712a214571fa5cc"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->decryptBlock(Cryptopp\HexUtils::hex2bin("974104846d0ad3ad7734ecb3ecee4eef"))));

// encrypt
echo "- encrypt:\n";
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"))));

// decrypt
echo "- decrypt:\n";
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("bd334f1d6e45f25ff712a214571fa5cc974104846d0ad3ad7734ecb3ecee4eef"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("ef7afd2270e2e60adce0ba2face6444e9a4b41ba738d6c72fb16691603c18e0e"))));

?>
--EXPECT--
- key length check:
bool(true)
bool(false)
bool(false)
- set key:
string(24) "azertyuiopqsdfghjklmwxcv"
- encrypt block:
string(32) "bd334f1d6e45f25ff712a214571fa5cc"
string(32) "974104846d0ad3ad7734ecb3ecee4eef"
- decrypt block:
string(32) "6bc1bee22e409f96e93d7e117393172a"
string(32) "ae2d8a571e03ac9c9eb76fac45af8e51"
- encrypt:
string(64) "bd334f1d6e45f25ff712a214571fa5cc974104846d0ad3ad7734ecb3ecee4eef"
string(64) "ef7afd2270e2e60adce0ba2face6444e9a4b41ba738d6c72fb16691603c18e0e"
- decrypt:
string(64) "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"
string(64) "30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"
