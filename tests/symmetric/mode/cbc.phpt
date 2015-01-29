--TEST--
Cipher mode: CBC
--FILE--
<?php

var_dump(is_a("Cryptopp\SymmetricModeCbc", "Cryptopp\SymmetricModeAbstract", true));

// check algorithm infos
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\SymmetricModeCbc($c);
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
$o = new Cryptopp\SymmetricModeCbc($c);
$o->setKey(hex2bin("2b7e151628aed2a6abf7158809cf4f3c"));
$o->setIv(hex2bin("000102030405060708090a0b0c0d0e0f"));
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"))));
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"))));

// decrypt
echo "- decrypt:\n";
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\SymmetricModeCbc($c);
$c->setKey(hex2bin("2b7e151628aed2a6abf7158809cf4f3c"));
$o->setIv(hex2bin("000102030405060708090a0b0c0d0e0f"));
var_dump(bin2hex($o->decrypt(hex2bin("7649abac8119b246cee98e9b12e9197d5086cb9b507219ee95db113a917678b2"))));
var_dump(bin2hex($o->decrypt(hex2bin("73bed6b8e3c1743b7116e69e222295163ff1caa1681fac09120eca307586e1a7"))));

// restart encryption
echo "- restart encryption:\n";
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"))));
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"))));

// TODO
// restart decryption
echo "- restart decryption:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("7649abac8119b246cee98e9b12e9197d5086cb9b507219ee95db113a917678b2"))));
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("73bed6b8e3c1743b7116e69e222295163ff1caa1681fac09120eca307586e1a7"))));

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485760);
var_dump(strlen($o->encrypt($data)));
var_dump(strlen($o->decrypt($data)));

?>
--EXPECT--
bool(true)
string(8) "cbc(aes)"
int(16)
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
string(64) "7649abac8119b246cee98e9b12e9197d5086cb9b507219ee95db113a917678b2"
string(64) "73bed6b8e3c1743b7116e69e222295163ff1caa1681fac09120eca307586e1a7"
- decrypt:
string(64) "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"
string(64) "30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"
- restart encryption:
string(64) "7649abac8119b246cee98e9b12e9197d5086cb9b507219ee95db113a917678b2"
string(64) "d9f6492663158e947664820606526b40961951dccb1de10f0437bea949a7c277"
- restart decryption:
string(64) "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"
string(64) "604fd5def72bfbf87829da2887712452f69f2445df4f9b17ad2b417be66c3710"
- large data:
int(10485760)
int(10485760)
