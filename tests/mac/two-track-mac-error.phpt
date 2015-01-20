--TEST--
MAC algorithm: Two-Track-Mac (errors)
--FILE--
<?php

$o = new Cryptopp\MacTwoTrackMac();
$o->setKey(hex2bin("00112233445566778899aabbccddeeff01234567"));

// invalid key
echo "- invalid key:\n";
try {
    $o->setKey("145");
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
$o = new Cryptopp\MacTwoTrackMac();

try {
    $o->calculateDigest("123456");
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
var_dump(@$o->calculateDigest(array()));
echo "$php_errormsg\n";
var_dump(@$o->update(array()));
echo "$php_errormsg\n";
var_dump(@$o->setKey(array()));
echo "$php_errormsg\n";

// test inheritance
echo "- inheritance:\n";
class Child extends Cryptopp\MacTwoTrackMac{}

$o = new Child();
var_dump($o->getDigestSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\MacTwoTrackMac
{
    public function __construct(){}
}

$o = new ChildParentConstructorNotCalled();

try {
    $o->getDigestSize();
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
- invalid key:
Cryptopp\MacTwoTrackMac : 3 is not a valid key length
Cryptopp\MacTwoTrackMac : a key is required
- no key:
Cryptopp\MacTwoTrackMac : a key is required
- sleep:
You cannot serialize or unserialize Cryptopp\MacAbstract instances
- bad arguments:
NULL
Cryptopp\MacAbstract::calculateDigest() expects parameter 1 to be string, array given
NULL
Cryptopp\MacAbstract::update() expects parameter 1 to be string, array given
NULL
Cryptopp\MacAbstract::setKey() expects parameter 1 to be string, array given
- inheritance:
int(20)
- inheritance (parent constructor not called):
Cryptopp\MacAbstract cannot be extended by user classes
