--TEST--
Cipher mode: ECB
--FILE--
<?php

var_dump(is_a("Cryptopp\SymmetricModeEcb", "Cryptopp\SymmetricModeAbstract", true));

// check that the class can be extended
class CipherChild extends Cryptopp\SymmetricModeEcb{}
$o = new CipherChild(new Cryptopp\BlockCipherAes());
var_dump($o->getName());

// check algorithm infos
$o = new Cryptopp\SymmetricModeEcb(new Cryptopp\BlockCipherAes());
var_dump($o->getName());
var_dump($o->getBlockSize());

// encrypt
echo "- encrypt:\n";
$o->setKey(hex2bin("2b7e151628aed2a6abf7158809cf4f3c"));
$o->setIv("1234567");
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"))));
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"))));

// decrypt
echo "- decrypt:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("3ad77bb40d7a3660a89ecaf32466ef97f5d3d58503b9699de785895a96fdbaaf"))));
var_dump(bin2hex($o->decrypt(hex2bin("43b1cd7f598ece23881b00e3ed0306887b0c785e27e8ad3f8223207104725dd4"))));

// restart
echo "- restart:\n";
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"))));
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"))));

// invalid key
echo "- invalid key:\n";
try {
    $o->setKey("123456789012345678901234567890123");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $o->setKey("");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without key
echo "- no key:\n";
$o = new Cryptopp\SymmetricModeEcb(new Cryptopp\BlockCipherAes());

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// sleep
echo "- sleep:\n";
try {
    serialize($o);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// test inheritance
echo "- inheritance:\n";
class Child extends Cryptopp\SymmetricModeEcb{}

$o = new Child(new Cryptopp\BlockCipherAes());
var_dump($o->getBlockSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\SymmetricModeEcb
{
    public function __construct(){}
}

$o = new ChildParentConstructorNotCalled();

try {
    $o->getBlockSize();
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
bool(true)
string(8) "ecb(aes)"
string(8) "ecb(aes)"
int(16)
- encrypt:
string(64) "3ad77bb40d7a3660a89ecaf32466ef97f5d3d58503b9699de785895a96fdbaaf"
string(64) "43b1cd7f598ece23881b00e3ed0306887b0c785e27e8ad3f8223207104725dd4"
- decrypt:
string(64) "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"
string(64) "30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"
- restart:
string(64) "3ad77bb40d7a3660a89ecaf32466ef97f5d3d58503b9699de785895a96fdbaaf"
string(64) "43b1cd7f598ece23881b00e3ed0306887b0c785e27e8ad3f8223207104725dd4"
- invalid key:
Cryptopp\SymmetricModeEcb : 33 is not a valid key length
Cryptopp\SymmetricModeEcb : a key is required
- no key:
Cryptopp\SymmetricModeEcb : a key is required
- sleep:
You cannot serialize or unserialize Cryptopp\SymmetricModeAbstract instances
- inheritance:
int(16)
- inheritance (parent constructor not called):
Cryptopp\SymmetricModeAbstract cannot be extended by user classes
