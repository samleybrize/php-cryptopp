--TEST--
Stream cipher: Panama (errors)
--FILE--
<?php

$o = new Cryptopp\StreamCipherPanama();
$o->setKey("00112233445566778899aabbccddeeff");
$o->setIv("00112233445566778899aabbccddeeff");

// invalid endianness
echo "- invalid endianness:\n";
try {
    new Cryptopp\StreamCipherPanama(5);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

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
$o = new Cryptopp\StreamCipherPanama();

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without iv
echo "- no iv:\n";
$o = new Cryptopp\StreamCipherPanama();
$o->setKey("00112233445566778899aabbccddeeff");

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
class Child extends Cryptopp\StreamCipherPanama{}

$o = new Child();
var_dump($o->getBlockSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\StreamCipherPanama
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
- invalid endianness:
Cryptopp\StreamCipherPanama: invalid endianness. Must be either Cryptopp\LITTLE_ENDIAN or Cryptopp\BIG_ENDIAN.
- invalid key:
Cryptopp\StreamCipherPanama : 33 is not a valid key length
Cryptopp\StreamCipherPanama : a key is required
- invalid iv:
Cryptopp\StreamCipherPanama : 3 is not a valid initialization vector length
Cryptopp\StreamCipherPanama : an initialization vector is required
- no key:
Cryptopp\StreamCipherPanama : a key is required
- no iv:
Cryptopp\StreamCipherPanama : an initialization vector is required
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
