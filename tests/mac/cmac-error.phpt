--TEST--
MAC algorithm: CMAC (errors)
--FILE--
<?php

$o = new Cryptopp\MacCmac(new Cryptopp\BlockCipherAes());
$o->setKey("1234567890123456");

// empty key
echo "- empty key:\n";
try {
    $o->setKey("");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without key
echo "- no key:\n";
$o = new Cryptopp\MacCmac(new Cryptopp\BlockCipherAes());

try {
    $o->calculateDigest("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// key not matching
echo "- key not matching:\n";
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\MacCmac($c);
$o->setKey("1234567890123456");
$c->setKey("6543210987654321");

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
class Child extends Cryptopp\MacCmac{}

$o = new Child(new Cryptopp\BlockCipherAes());
var_dump($o->getDigestSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\MacCmac
{
    public function __construct(){}
}

$o = new ChildParentConstructorNotCalled(new Cryptopp\BlockCipherAes());

try {
    $o->getDigestSize();
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
- empty key:
Cryptopp\MacCmac : a key is required
- no key:
Cryptopp\MacCmac : a key is required
- key not matching:
Cryptopp\MacCmac: key is not matching the one owned by the underlying object
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
int(16)
- inheritance (parent constructor not called):
Cryptopp\MacAbstract cannot be extended by user classes
