--TEST--
Stream cipher: XSalsa20 (errors)
--FILE--
<?php

$o = new Cryptopp\StreamCipherXSalsa20();
$o->setKey(Cryptopp\HexUtils::hex2bin("a7c083feb7aabbff1122334455667788a7c083feb7aabbff1122334455667788"));
$o->setIv(Cryptopp\HexUtils::hex2bin("00112233445566778899AABBCCDDFF001122334455667788"));

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

try {
    $o->setIv("");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without key
echo "- no key:\n";
$o = new Cryptopp\StreamCipherXSalsa20();

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without iv
echo "- no iv:\n";
$o = new Cryptopp\StreamCipherXSalsa20();
$o->setKey("a7c083feb7aabbff1122334455667788");

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// invalid rounds
echo "- invalid rounds:\n";
$o = new Cryptopp\StreamCipherXSalsa20();

try {
    $o->setRounds(13);
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
class Child extends Cryptopp\StreamCipherXSalsa20{}

$o = new Child();
var_dump($o->getBlockSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\StreamCipherXSalsa20
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
Cryptopp\StreamCipherXSalsa20 : 33 is not a valid key length
Cryptopp\StreamCipherXSalsa20 : a key is required
- invalid iv:
Cryptopp\StreamCipherXSalsa20 : 3 is not a valid initialization vector length
Cryptopp\StreamCipherXSalsa20 : an initialization vector is required
- no key:
Cryptopp\StreamCipherXSalsa20 : a key is required
- no iv:
Cryptopp\StreamCipherXSalsa20 : an initialization vector is required
- invalid rounds:
Cryptopp\StreamCipherXSalsa20: number of rounds must be one of 8, 12 or 20
- sleep:
You cannot serialize or unserialize Cryptopp\StreamCipherAbstract instances
- bad arguments:
NULL
Cryptopp\StreamCipherAbstract::encrypt() expects parameter 1 to be string, array given
NULL
Cryptopp\StreamCipherAbstract::decrypt() expects parameter 1 to be string, array given
NULL
Cryptopp\StreamCipherAbstract::setKey() expects parameter 1 to be string, array given
NULL
Cryptopp\StreamCipherAbstract::setIv() expects parameter 1 to be string, array given
- inheritance:
int(1)
- inheritance (parent constructor not called):
Cryptopp\StreamCipherAbstract cannot be extended by user classes
