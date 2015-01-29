--TEST--
Cipher mode: CFB
--FILE--
<?php

var_dump(is_a("Cryptopp\SymmetricModeCfb", "Cryptopp\SymmetricModeAbstract", true));

// check algorithm infos
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\SymmetricModeCfb($c);
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
$o = new Cryptopp\SymmetricModeCfb($c);
$o->setKey(hex2bin("2b7e151628aed2a6abf7158809cf4f3c"));
$o->setIv(hex2bin("000102030405060708090a0b0c0d0e0f"));
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"))));
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"))));

// decrypt
echo "- decrypt:\n";
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\SymmetricModeCfb($c);
$c->setKey(hex2bin("2b7e151628aed2a6abf7158809cf4f3c"));
$o->setIv(hex2bin("000102030405060708090a0b0c0d0e0f"));
var_dump(bin2hex($o->decrypt(hex2bin("3b3fd92eb72dad20333449f8e83cfb4ac8a64537a0b3a93fcde3cdad9f1ce58b"))));
var_dump(bin2hex($o->decrypt(hex2bin("26751f67a3cbb140b1808cf187a4f4dfc04b05357c5d1c0eeac4c66f9ff7f2e6"))));

// restart encryption
echo "- restart encryption:\n";
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"))));
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"))));

// restart decryption
echo "- restart decryption:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("3b3fd92eb72dad20333449f8e83cfb4ac8a64537a0b3a93fcde3cdad9f1ce58b"))));
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("26751f67a3cbb140b1808cf187a4f4dfc04b05357c5d1c0eeac4c66f9ff7f2e6"))));

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485758);
var_dump(strlen($o->encrypt($data)));
var_dump(strlen($o->decrypt($data)));

?>
--EXPECT--
bool(true)
string(8) "cfb(aes)"
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
string(64) "3b3fd92eb72dad20333449f8e83cfb4ac8a64537a0b3a93fcde3cdad9f1ce58b"
string(64) "26751f67a3cbb140b1808cf187a4f4dfc04b05357c5d1c0eeac4c66f9ff7f2e6"
- decrypt:
string(64) "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"
string(64) "30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"
- restart encryption:
string(64) "3b3fd92eb72dad20333449f8e83cfb4ac8a64537a0b3a93fcde3cdad9f1ce58b"
string(64) "60367b8a3a31d6a73ff2f6f081a5be8f6c01b8a6e488e0e862c412c6f630ca52"
- restart decryption:
string(64) "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"
string(64) "768b78ab3aa683f66b89bb181c0b18bff69f2445df4f9b17ad2b417be66c3710"
- large data:
int(10485758)
int(10485758)
