--TEST--
Cipher mode: CBC (errors)
--FILE--
<?php

$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\SymmetricModeCbc($c);
$c->setKey(Cryptopp\HexUtils::hex2bin("2b7e151628aed2a6abf7158809cf4f3c"));

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

// invalid iv
echo "- invalid iv:\n";
try {
    $o->setIv("123");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without key
echo "- no key:\n";
$o = new Cryptopp\SymmetricModeCbc(new Cryptopp\BlockCipherAes());

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without iv
echo "- no iv:\n";
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\SymmetricModeCbc($c);
$c->setKey(Cryptopp\HexUtils::hex2bin("2b7e151628aed2a6abf7158809cf4f3c"));

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

// bad arguments
echo "- bad arguments:\n";
var_dump(@$o->encrypt(array()));
echo "$php_errormsg\n";
var_dump(@$o->decrypt(array()));
echo "$php_errormsg\n";
var_dump(@$o->setKey(array()));
echo "$php_errormsg\n";
var_dump(@$o->setIv(array()));
echo "$php_errormsg\n";

// test inheritance
echo "- inheritance:\n";
class Child extends Cryptopp\SymmetricModeCbc{}

$o = new Child(new Cryptopp\BlockCipherAes());
var_dump($o->getBlockSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\SymmetricModeCbc
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
- invalid key:
Cryptopp\SymmetricModeCbc : 33 is not a valid key length
Cryptopp\SymmetricModeCbc : a key is required
- invalid iv:
Cryptopp\SymmetricModeCbc : 3 is not a valid initialization vector length
- no key:
Cryptopp\SymmetricModeCbc : a key is required
- no iv:
Cryptopp\SymmetricModeCbc : an initialization vector is required
- sleep:
You cannot serialize or unserialize Cryptopp\SymmetricModeAbstract instances
- bad arguments:
NULL
Cryptopp\SymmetricModeAbstract::encrypt() expects parameter 1 to be string, array given
NULL
Cryptopp\SymmetricModeAbstract::decrypt() expects parameter 1 to be string, array given
NULL
Cryptopp\SymmetricModeAbstract::setKey() expects parameter 1 to be string, array given
NULL
Cryptopp\SymmetricModeAbstract::setIv() expects parameter 1 to be string, array given
- inheritance:
int(16)
- inheritance (parent constructor not called):
Cryptopp\SymmetricModeAbstract cannot be extended by user classes
