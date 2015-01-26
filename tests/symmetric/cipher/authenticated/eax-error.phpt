--TEST--
Authenticated symmetric cipher: EAX (errors)
--FILE--
<?php

$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\AuthenticatedSymmetricCipherEax($c);
$o->setKey(hex2bin("77be63708971c4e240d1cb79e8d77feb"));
$o->setIv(hex2bin("e0e00f19fed7ba0136a797f3"));

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
echo "- encrypt no key:\n";
$o = new Cryptopp\AuthenticatedSymmetricCipherEax(new Cryptopp\BlockCipherAes());

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// aad without key
echo "- aad no key:\n";
$o = new Cryptopp\AuthenticatedSymmetricCipherEax(new Cryptopp\BlockCipherAes());

try {
    $o->addEncryptionAdditionalData("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// invalid iv
echo "- invalid iv:\n";
try {
    $o->setIv("123");
    echo "iv size 3 ok\n";
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt before aad
echo "- encrypt before aad:\n";
$o->restart();
$o->setKey("1234567890123456");
$o->setIv("1234567890123456");

$o->encrypt("azerty");

try {
    $o->addEncryptionAdditionalData("qwerty");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// decrypt before aad
echo "- decrypt before aad:\n";
$o->decrypt("azerty");

try {
    $o->addDecryptionAdditionalData("qwerty");
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
var_dump(@$o->addEncryptionAdditionalData(array()));
echo "$php_errormsg\n";
var_dump(@$o->addDecryptionAdditionalData(array()));
echo "$php_errormsg\n";
var_dump(@$o->setKey(array()));
echo "$php_errormsg\n";
var_dump(@$o->setIv(array()));
echo "$php_errormsg\n";

// test inheritance
echo "- inheritance:\n";
class Child extends Cryptopp\AuthenticatedSymmetricCipherEax{}

$o = new Child(new Cryptopp\BlockCipherAes());
var_dump($o->getBlockSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\AuthenticatedSymmetricCipherEax
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
Cryptopp\AuthenticatedSymmetricCipherEax : 33 is not a valid key length
Cryptopp\AuthenticatedSymmetricCipherEax : a key is required
- encrypt no key:
Cryptopp\AuthenticatedSymmetricCipherEax : a key is required
- aad no key:
Cryptopp\AuthenticatedSymmetricCipherEax : a key is required
- invalid iv:
iv size 3 ok
- encrypt before aad:
Cryptopp\AuthenticatedSymmetricCipherEax: additional authenticated data must be added before any encryption
- decrypt before aad:
Cryptopp\AuthenticatedSymmetricCipherEax: additional authenticated data must be added before any decryption
- sleep:
You cannot serialize or unserialize Cryptopp\AuthenticatedSymmetricCipherAbstract instances
- bad arguments:
NULL
Cryptopp\AuthenticatedSymmetricCipherAbstract::encrypt() expects parameter 1 to be string, array given
NULL
Cryptopp\AuthenticatedSymmetricCipherAbstract::decrypt() expects parameter 1 to be string, array given
NULL
Cryptopp\AuthenticatedSymmetricCipherAbstract::addEncryptionAdditionalData() expects parameter 1 to be string, array given
NULL
Cryptopp\AuthenticatedSymmetricCipherAbstract::addDecryptionAdditionalData() expects parameter 1 to be string, array given
NULL
Cryptopp\AuthenticatedSymmetricCipherAbstract::setKey() expects parameter 1 to be string, array given
NULL
Cryptopp\AuthenticatedSymmetricCipherAbstract::setIv() expects parameter 1 to be string, array given
- inheritance:
int(1)
- inheritance (parent constructor not called):
Cryptopp\AuthenticatedSymmetricCipherAbstract cannot be extended by user classes
