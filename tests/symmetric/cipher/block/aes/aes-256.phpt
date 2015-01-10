--TEST--
Block cipher: AES-256
--FILE--
<?php

$o = new Cryptopp\BlockCipherAes();

// key length check
echo "- key length check:\n";
var_dump($o->isValidKeyLength(32));
var_dump($o->isValidKeyLength(31));
var_dump($o->isValidKeyLength(33));

// set key
echo "- set key:\n";
$o->setKey("azertyuiopqsdfghjklmwxcvazertyui");
var_dump($o->getKey());

// encryptBlock
echo "- encrypt block:\n";
$o->setKey(hex2bin("603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4"));
var_dump(bin2hex($o->encryptBlock(hex2bin("6bc1bee22e409f96e93d7e117393172a"))));
var_dump(bin2hex($o->encryptBlock(hex2bin("ae2d8a571e03ac9c9eb76fac45af8e51"))));

// decryptBlock
echo "- decrypt block:\n";
var_dump(bin2hex($o->decryptBlock(hex2bin("f3eed1bdb5d2a03c064b5a7e3db181f8"))));
var_dump(bin2hex($o->decryptBlock(hex2bin("591ccb10d410ed26dc5ba74a31362870"))));

// encrypt
echo "- encrypt:\n";
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"))));
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"))));

// decrypt
echo "- decrypt:\n";
var_dump(bin2hex($o->decrypt(hex2bin("f3eed1bdb5d2a03c064b5a7e3db181f8591ccb10d410ed26dc5ba74a31362870"))));
var_dump(bin2hex($o->decrypt(hex2bin("b6ed21b99ca6f4f9f153e7b1beafed1d23304b7a39f9f3ff067d8d8f9e24ecc7"))));

?>
--EXPECT--
- key length check:
bool(true)
bool(false)
bool(false)
- set key:
string(32) "azertyuiopqsdfghjklmwxcvazertyui"
- encrypt block:
string(32) "f3eed1bdb5d2a03c064b5a7e3db181f8"
string(32) "591ccb10d410ed26dc5ba74a31362870"
- decrypt block:
string(32) "6bc1bee22e409f96e93d7e117393172a"
string(32) "ae2d8a571e03ac9c9eb76fac45af8e51"
- encrypt:
string(64) "f3eed1bdb5d2a03c064b5a7e3db181f8591ccb10d410ed26dc5ba74a31362870"
string(64) "b6ed21b99ca6f4f9f153e7b1beafed1d23304b7a39f9f3ff067d8d8f9e24ecc7"
- decrypt:
string(64) "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"
string(64) "30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"
