--TEST--
Authenticated symmetric cipher: CCM (errors)
--FILE--
<?php

$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\AuthenticatedSymmetricCipherCcm($c);
$o->setKey(Cryptopp\HexUtils::hex2bin("77be63708971c4e240d1cb79e8d77feb"));
$o->setIv(Cryptopp\HexUtils::hex2bin("e0e00f19fed7ba0136a797f3"));

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
$o = new Cryptopp\AuthenticatedSymmetricCipherCcm(new Cryptopp\BlockCipherAes());

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// aad without key
echo "- aad no key:\n";
$o = new Cryptopp\AuthenticatedSymmetricCipherCcm(new Cryptopp\BlockCipherAes());

try {
    $o->addEncryptionAdditionalData("123456");
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

// encrypt before aad
echo "- encrypt before aad:\n";
$o->restart();
$o->setKey("1234567890123456");
$o->setIv("1234567890123");
$o->specifyDataSize(6, 0);
$o->setDigestSize(16);

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

// data length
echo "- data length:\n";
$o->specifyDataSize(10, 0);

try {
    $o->encrypt("12345678901");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $o->decrypt("12345678901");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $o->encrypt("123456789");
    $o->finalizeEncryption();
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $o->decrypt("123456789");
    $o->finalizeDecryption("123456789");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// aad length
echo "- aad length:\n";
$o->specifyDataSize(0, 10);

try {
    $o->addEncryptionAdditionalData("12345678901");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $o->addDecryptionAdditionalData("12345678901");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $o->addEncryptionAdditionalData("123456789");
    $o->finalizeEncryption();
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $o->addDecryptionAdditionalData("123456789");
    $o->finalizeDecryption();
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// specifyDataLength before setKey/setIv
echo "- specifyDataSize before key/iv:\n";
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\AuthenticatedSymmetricCipherCcm($c);

try {
    $o->specifyDataSize(14, 12);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// setDigestSize before setKey/setIv
echo "- setDigestSize before key/iv:\n";
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\AuthenticatedSymmetricCipherCcm($c);

try {
    $o->setDigestSize(14);
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
var_dump(@$o->setDigestSize(array()));
echo "$php_errormsg\n";
var_dump(@$o->specifyDataSize(array(), 0));
echo "$php_errormsg\n";
var_dump(@$o->specifyDataSize(0, array()));
echo "$php_errormsg\n";

// test inheritance
echo "- inheritance:\n";
class Child extends Cryptopp\AuthenticatedSymmetricCipherCcm{}

$o = new Child(new Cryptopp\BlockCipherAes());
var_dump($o->getBlockSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\AuthenticatedSymmetricCipherCcm
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
Cryptopp\AuthenticatedSymmetricCipherCcm : 33 is not a valid key length
Cryptopp\AuthenticatedSymmetricCipherCcm : a key is required
- encrypt no key:
Cryptopp\AuthenticatedSymmetricCipherCcm : a key is required
- aad no key:
Cryptopp\AuthenticatedSymmetricCipherCcm : a key is required
- invalid iv:
Cryptopp\AuthenticatedSymmetricCipherCcm : 3 is not a valid initialization vector length
- encrypt before aad:
Cryptopp\AuthenticatedSymmetricCipherCcm: additional authenticated data must be added before any encryption
- decrypt before aad:
Cryptopp\AuthenticatedSymmetricCipherCcm: additional authenticated data must be added before any decryption
- data length:
Cryptopp\AuthenticatedSymmetricCipherCcm : message length doesn't match that given in specifyDataSize (10 expected, 11 given)
Cryptopp\AuthenticatedSymmetricCipherCcm : message length doesn't match that given in specifyDataSize (10 expected, 11 given)
Cryptopp\AuthenticatedSymmetricCipherCcm : message length doesn't match that given in specifyDataSize (10 expected, 9 given)
Cryptopp\AuthenticatedSymmetricCipherCcm : message length doesn't match that given in specifyDataSize (10 expected, 9 given)
- aad length:
Cryptopp\AuthenticatedSymmetricCipherCcm : AAD length doesn't match that given in specifyDataSize (10 expected, 11 given)
Cryptopp\AuthenticatedSymmetricCipherCcm : AAD length doesn't match that given in specifyDataSize (10 expected, 11 given)
Cryptopp\AuthenticatedSymmetricCipherCcm : AAD length doesn't match that given in specifyDataSize (10 expected, 9 given)
Cryptopp\AuthenticatedSymmetricCipherCcm : AAD length doesn't match that given in specifyDataSize (10 expected, 9 given)
- specifyDataSize before key/iv:
Cryptopp\AuthenticatedSymmetricCipherCcm : a key is required
- setDigestSize before key/iv:
Cryptopp\AuthenticatedSymmetricCipherCcm : a key is required
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
Cryptopp\AuthenticatedSymmetricCipherCcm::setDigestSize() expects parameter 1 to be long, array given
NULL
Cryptopp\AuthenticatedSymmetricCipherCcm::specifyDataSize() expects parameter 1 to be long, array given
NULL
Cryptopp\AuthenticatedSymmetricCipherCcm::specifyDataSize() expects parameter 2 to be long, array given
- inheritance:
int(1)
- inheritance (parent constructor not called):
Cryptopp\AuthenticatedSymmetricCipherAbstract cannot be extended by user classes
