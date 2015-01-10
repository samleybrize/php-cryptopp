--TEST--
Stream cipher: Sosemanuk
--FILE--
<?php

var_dump(is_a("Cryptopp\StreamCipherSosemanuk", "Cryptopp\StreamCipherAbstract", true));

// check algorithm infos
$o = new Cryptopp\StreamCipherSosemanuk();
var_dump($o->getName());
var_dump($o->getBlockSize());

// key length check
echo "- key length check:\n";
var_dump($o->isValidKeyLength(16));
var_dump($o->isValidKeyLength(23));
var_dump($o->isValidKeyLength(9));
var_dump($o->isValidKeyLength(0));
var_dump($o->isValidKeyLength(33));

// iv length check
echo "- iv length check:\n";
var_dump($o->isValidIvLength(16));
var_dump($o->isValidIvLength(15));
var_dump($o->isValidIvLength(17));

// set key
echo "- set key:\n";
$o->setKey("azertyuiopqsdfgh");
var_dump($o->getKey());

// set iv
echo "- set iv:\n";
$o->setIv("qsdfghjklmazerty");
var_dump($o->getIv());

// encrypt
echo "- encrypt:\n";
$o->setKey(hex2bin("a7c083feb7"));
$o->setIv(hex2bin("00112233445566778899aabbccddeeff"));
var_dump(bin2hex($o->encrypt(hex2bin("00000000000000000000000000000000"))));
var_dump(bin2hex($o->encrypt(hex2bin("00000000000000000000000000000000"))));

// decrypt
echo "- decrypt:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("fe81d2162c9a100d04895c454a77515b"))));
var_dump(bin2hex($o->decrypt(hex2bin("be6a431a935cb90e2221ebb7ef502328"))));

// restart encryption
echo "- restart encryption:\n";
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("00000000000000000000000000000000"))));
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("00000000000000000000000000000000"))));

// restart decryption
echo "- restart decryption:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("fe81d2162c9a100d04895c454a77515b"))));
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("be6a431a935cb90e2221ebb7ef502328"))));

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
$o = new Cryptopp\StreamCipherSosemanuk();

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without iv
echo "- no iv:\n";
$o = new Cryptopp\StreamCipherSosemanuk();
$o->setKey("12345");

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
class Child extends Cryptopp\StreamCipherSosemanuk{}

$o = new Child();
var_dump($o->getBlockSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\StreamCipherSosemanuk
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
string(9) "sosemanuk"
int(1)
- key length check:
bool(true)
bool(true)
bool(true)
bool(false)
bool(false)
- iv length check:
bool(true)
bool(false)
bool(false)
- set key:
string(16) "azertyuiopqsdfgh"
- set iv:
string(16) "qsdfghjklmazerty"
- encrypt:
string(32) "fe81d2162c9a100d04895c454a77515b"
string(32) "be6a431a935cb90e2221ebb7ef502328"
- decrypt:
string(32) "00000000000000000000000000000000"
string(32) "00000000000000000000000000000000"
- restart encryption:
string(32) "fe81d2162c9a100d04895c454a77515b"
string(32) "fe81d2162c9a100d04895c454a77515b"
- restart decryption:
string(32) "00000000000000000000000000000000"
string(32) "40eb910cbfc6a90326a8b7f2a5277273"
- invalid key:
Cryptopp\StreamCipherSosemanuk : 33 is not a valid key length
Cryptopp\StreamCipherSosemanuk : a key is required
- invalid iv:
Cryptopp\StreamCipherSosemanuk : 3 is not a valid initialization vector length
Cryptopp\StreamCipherSosemanuk : an initialization vector is required
- no key:
Cryptopp\StreamCipherSosemanuk : a key is required
- no iv:
Cryptopp\StreamCipherSosemanuk : an initialization vector is required
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
