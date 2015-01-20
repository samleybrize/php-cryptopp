--TEST--
Authenticated symmetric cipher generic (errors)
--FILE--
<?php

$c = new Cryptopp\StreamCipherSosemanuk();
$m = new Cryptopp\MacHmac(new Cryptopp\HashSha1());
$o = new Cryptopp\AuthenticatedSymmetricCipherGeneric($c, $m);
$o->setKey(hex2bin("77be637089"));
$o->setIv(hex2bin("e0e00f19fed7ba0136a797f3ed7ba013"));
$o->setMacKey(hex2bin("feffe9928665731c6d6a8f9467308308"));

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
$c = new Cryptopp\StreamCipherSosemanuk();
$m = new Cryptopp\MacHmac(new Cryptopp\HashSha1());
$o = new Cryptopp\AuthenticatedSymmetricCipherGeneric($c, $m);

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without iv
echo "- no iv:\n";
$c = new Cryptopp\StreamCipherSosemanuk();
$m = new Cryptopp\MacHmac(new Cryptopp\HashSha1());
$o = new Cryptopp\AuthenticatedSymmetricCipherGeneric($c, $m);
$o->setKey("12345");

try {
    $o->encrypt("123456");
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
var_dump(@$o->setMacKey(array()));
echo "$php_errormsg\n";

// test inheritance
echo "- inheritance:\n";
class Child extends Cryptopp\AuthenticatedSymmetricCipherGeneric{}

$o = new Child($c, $m);
var_dump($o->getBlockSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\AuthenticatedSymmetricCipherGeneric
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
Cryptopp\AuthenticatedSymmetricCipherGeneric : 33 is not a valid key length
Cryptopp\AuthenticatedSymmetricCipherGeneric : a key is required
- invalid iv:
Cryptopp\AuthenticatedSymmetricCipherGeneric : 3 is not a valid initialization vector length
Cryptopp\AuthenticatedSymmetricCipherGeneric : an initialization vector is required
- no key:
Cryptopp\AuthenticatedSymmetricCipherGeneric : a key is required
- no iv:
Cryptopp\AuthenticatedSymmetricCipherGeneric : an initialization vector is required
- encrypt before aad:
Cryptopp\AuthenticatedSymmetricCipherGeneric: additional authenticated data must be added before any encryption
- decrypt before aad:
Cryptopp\AuthenticatedSymmetricCipherGeneric: additional authenticated data must be added before any decryption
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
NULL
Cryptopp\AuthenticatedSymmetricCipherGeneric::setMacKey() expects parameter 1 to be string, array given
- inheritance:
int(1)
- inheritance (parent constructor not called):
Cryptopp\AuthenticatedSymmetricCipherAbstract cannot be extended by user classes
