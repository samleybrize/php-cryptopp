--TEST--
Cipher mode: OFB
--FILE--
<?php

var_dump(is_a("Cryptopp\SymmetricModeOfb", "Cryptopp\SymmetricModeAbstract", true));

// check algorithm infos
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\SymmetricModeOfb($c);
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
$o = new Cryptopp\SymmetricModeOfb($c);
$o->setKey(hex2bin("2b7e151628aed2a6abf7158809cf4f3c"));
$o->setIv(hex2bin("000102030405060708090a0b0c0d0e0f"));
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"))));
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"))));

// decrypt
echo "- decrypt:\n";
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\SymmetricModeOfb($c);
$c->setKey(hex2bin("2b7e151628aed2a6abf7158809cf4f3c"));
$o->setIv(hex2bin("000102030405060708090a0b0c0d0e0f"));
var_dump(bin2hex($o->decrypt(hex2bin("3b3fd92eb72dad20333449f8e83cfb4a7789508d16918f03f53c52dac54ed825"))));
var_dump(bin2hex($o->decrypt(hex2bin("9740051e9c5fecf64344f7a82260edcc304c6528f659c77866a510d9c1d6ae5e"))));

// restart encryption
echo "- restart encryption:\n";
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"))));
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"))));

// restart decryption
echo "- restart decryption:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("3b3fd92eb72dad20333449f8e83cfb4a7789508d16918f03f53c52dac54ed825"))));
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("9740051e9c5fecf64344f7a82260edcc304c6528f659c77866a510d9c1d6ae5e"))));

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485758);
var_dump(strlen($o->encrypt($data)));
var_dump(strlen($o->decrypt($data)));

?>
--EXPECT--
bool(true)
string(8) "ofb(aes)"
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
string(64) "3b3fd92eb72dad20333449f8e83cfb4a7789508d16918f03f53c52dac54ed825"
string(64) "9740051e9c5fecf64344f7a82260edcc304c6528f659c77866a510d9c1d6ae5e"
- decrypt:
string(64) "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"
string(64) "30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"
- restart encryption:
string(64) "3b3fd92eb72dad20333449f8e83cfb4a7789508d16918f03f53c52dac54ed825"
string(64) "60367b8a3a31d6a73ff2f6f081a5be8f2f3bfe9fd7ddb888c6a07c0d668d6164"
- restart decryption:
string(64) "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"
string(64) "c7be62d20532de40994dc041b9cf01ace9e8bff2fecbe4e70d2e2daf4137f82a"
- large data:
int(10485758)
int(10485758)
