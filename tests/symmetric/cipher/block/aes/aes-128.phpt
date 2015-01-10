--TEST--
Block cipher: AES-128
--FILE--
<?php

$o = new Cryptopp\BlockCipherAes();

// key length check
echo "- key length check:\n";
var_dump($o->isValidKeyLength(16));
var_dump($o->isValidKeyLength(15));
var_dump($o->isValidKeyLength(17));

// set key
echo "- set key:\n";
$o->setKey("azertyuiopqsdfgh");
var_dump($o->getKey());

// encryptBlock
echo "- encrypt block:\n";
$o->setKey(hex2bin("2b7e151628aed2a6abf7158809cf4f3c"));
var_dump(bin2hex($o->encryptBlock(hex2bin("6bc1bee22e409f96e93d7e117393172a"))));
var_dump(bin2hex($o->encryptBlock(hex2bin("ae2d8a571e03ac9c9eb76fac45af8e51"))));

// decryptBlock
echo "- decrypt block:\n";
var_dump(bin2hex($o->decryptBlock(hex2bin("3ad77bb40d7a3660a89ecaf32466ef97"))));
var_dump(bin2hex($o->decryptBlock(hex2bin("f5d3d58503b9699de785895a96fdbaaf"))));

// encrypt
echo "- encrypt:\n";
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"))));
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"))));

// decrypt
echo "- decrypt:\n";
var_dump(bin2hex($o->decrypt(hex2bin("3ad77bb40d7a3660a89ecaf32466ef97f5d3d58503b9699de785895a96fdbaaf"))));
var_dump(bin2hex($o->decrypt(hex2bin("43b1cd7f598ece23881b00e3ed0306887b0c785e27e8ad3f8223207104725dd4"))));

?>
--EXPECT--
- key length check:
bool(true)
bool(false)
bool(false)
- set key:
string(16) "azertyuiopqsdfgh"
- encrypt block:
string(32) "3ad77bb40d7a3660a89ecaf32466ef97"
string(32) "f5d3d58503b9699de785895a96fdbaaf"
- decrypt block:
string(32) "6bc1bee22e409f96e93d7e117393172a"
string(32) "ae2d8a571e03ac9c9eb76fac45af8e51"
- encrypt:
string(64) "3ad77bb40d7a3660a89ecaf32466ef97f5d3d58503b9699de785895a96fdbaaf"
string(64) "43b1cd7f598ece23881b00e3ed0306887b0c785e27e8ad3f8223207104725dd4"
- decrypt:
string(64) "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"
string(64) "30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"
