--TEST--
Cipher mode: ECB
--FILE--
<?php

var_dump(is_a("Cryptopp\SymmetricModeEcb", "Cryptopp\SymmetricModeAbstract", true));

// check algorithm infos
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\SymmetricModeEcb($c);
var_dump($o->getName());
var_dump($o->getBlockSize());

// key length check
echo "- key length check:\n";
var_dump($o->isValidKeyLength(16));
var_dump($o->isValidKeyLength(15));
var_dump($o->isValidKeyLength(17));

// iv length check
echo "- iv length check:\n";
var_dump($o->isValidIvLength(0));
var_dump($o->isValidIvLength(2));
var_dump($o->isValidIvLength(4));

// set key
echo "- set key:\n";
$o->setKey("azertyuiopqsdfgh");
var_dump($o->getKey());
var_dump($c->getKey());

// set iv
echo "- set iv:\n";
$o->setIv("");
var_dump($o->getIv());

// encrypt
echo "- encrypt:\n";
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\SymmetricModeEcb($c);
$c->setKey(hex2bin("2b7e151628aed2a6abf7158809cf4f3c"));
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"))));
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"))));

// decrypt
echo "- decrypt:\n";
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\SymmetricModeEcb($c);
$c->setKey(hex2bin("2b7e151628aed2a6abf7158809cf4f3c"));
var_dump(bin2hex($o->decrypt(hex2bin("3ad77bb40d7a3660a89ecaf32466ef97f5d3d58503b9699de785895a96fdbaaf"))));
var_dump(bin2hex($o->decrypt(hex2bin("43b1cd7f598ece23881b00e3ed0306887b0c785e27e8ad3f8223207104725dd4"))));

// restart encryption
echo "- restart encryption:\n";
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"))));
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"))));

// restart decryption
echo "- restart decryption:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("3ad77bb40d7a3660a89ecaf32466ef97f5d3d58503b9699de785895a96fdbaaf"))));
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("43b1cd7f598ece23881b00e3ed0306887b0c785e27e8ad3f8223207104725dd4"))));

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485760);
var_dump(strlen($o->encrypt($data)));
var_dump(strlen($o->decrypt($data)));

?>
--EXPECT--
bool(true)
string(8) "ecb(aes)"
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
string(0) ""
- encrypt:
string(64) "3ad77bb40d7a3660a89ecaf32466ef97f5d3d58503b9699de785895a96fdbaaf"
string(64) "43b1cd7f598ece23881b00e3ed0306887b0c785e27e8ad3f8223207104725dd4"
- decrypt:
string(64) "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"
string(64) "30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"
- restart encryption:
string(64) "3ad77bb40d7a3660a89ecaf32466ef97f5d3d58503b9699de785895a96fdbaaf"
string(64) "43b1cd7f598ece23881b00e3ed0306887b0c785e27e8ad3f8223207104725dd4"
- restart decryption:
string(64) "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"
string(64) "30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"
- large data:
int(10485760)
int(10485760)
