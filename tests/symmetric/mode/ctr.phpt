--TEST--
Cipher mode: CTR
--FILE--
<?php

var_dump(is_a("Cryptopp\SymmetricModeCtr", "Cryptopp\SymmetricModeAbstract", true));

// check algorithm infos
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\SymmetricModeCtr($c);
var_dump($o->getName());
var_dump($o->getBlockSize());

// key length check
echo "- key length check:\n";
var_dump($o->isValidKeyLength(16));
var_dump($o->isValidKeyLength(15));
var_dump($o->isValidKeyLength(17));

// iv length check
echo "- iv length check:\n";
var_dump($o->isValidIvLength(16));
var_dump($o->isValidIvLength(2));
var_dump($o->isValidIvLength(24));

// set key
echo "- set key:\n";
$o->setKey("azertyuiopqsdfgh");
var_dump($o->getKey());
var_dump($c->getKey());

// set iv
echo "- set iv:\n";
$o->setIv("mlkjhgfdsqnbvcxw");
var_dump($o->getIv());

// encrypt
echo "- encrypt:\n";
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\SymmetricModeCtr($c);
$o->setKey(hex2bin("2b7e151628aed2a6abf7158809cf4f3c"));
$o->setIv(hex2bin("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"));
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"))));
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"))));

// decrypt
echo "- decrypt:\n";
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\SymmetricModeCtr($c);
$c->setKey(hex2bin("2b7e151628aed2a6abf7158809cf4f3c"));
$o->setIv(hex2bin("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"));
var_dump(bin2hex($o->decrypt(hex2bin("874d6191b620e3261bef6864990db6ce9806f66b7970fdff8617187bb9fffdff"))));
var_dump(bin2hex($o->decrypt(hex2bin("5ae4df3edbd5d35e5b4f09020db03eab1e031dda2fbe03d1792170a0f3009cee"))));

// restart encryption
echo "- restart encryption:\n";
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"))));
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"))));

// restart decryption
echo "- restart decryption:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("874d6191b620e3261bef6864990db6ce9806f66b7970fdff8617187bb9fffdff"))));
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("5ae4df3edbd5d35e5b4f09020db03eab1e031dda2fbe03d1792170a0f3009cee"))));

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485758);
var_dump(strlen($o->encrypt($data)));
var_dump(strlen($o->decrypt($data)));

?>
--EXPECT--
bool(true)
string(8) "ctr(aes)"
int(1)
- key length check:
bool(true)
bool(false)
bool(false)
- iv length check:
bool(true)
bool(false)
bool(false)
- set key:
string(16) "azertyuiopqsdfgh"
string(16) "azertyuiopqsdfgh"
- set iv:
string(16) "mlkjhgfdsqnbvcxw"
- encrypt:
string(64) "874d6191b620e3261bef6864990db6ce9806f66b7970fdff8617187bb9fffdff"
string(64) "5ae4df3edbd5d35e5b4f09020db03eab1e031dda2fbe03d1792170a0f3009cee"
- decrypt:
string(64) "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"
string(64) "30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"
- restart encryption:
string(64) "874d6191b620e3261bef6864990db6ce9806f66b7970fdff8617187bb9fffdff"
string(64) "dc44c3353b3c98a11729d76cf094f30bc0b45879b83cca74b58b36ac1a3c44be"
- restart decryption:
string(64) "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"
string(64) "b668004d43b5afeea99d1f77e72e9f4f282861e648cd52b2618107770f50ef40"
- large data:
int(10485758)
int(10485758)
