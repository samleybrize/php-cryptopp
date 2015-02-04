--TEST--
MAC algorithm: HMAC (errors)
--FILE--
<?php

$o = new Cryptopp\MacHmac(new Cryptopp\HashMd5());
$o->setKey(Cryptopp\HexUtils::hex2bin("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b"));

// empty key
echo "- empty key:\n";
try {
    $o->setKey("");
    echo "empty key ok\n";
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without key
echo "- no key:\n";
$o = new Cryptopp\MacHmac(new Cryptopp\HashSha1());

try {
    $o->calculateDigest("123456");
    echo "no key ok\n";
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
class Child extends Cryptopp\MacHmac{}

$o = new Child(new Cryptopp\HashSha1());
var_dump($o->getDigestSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\MacHmac
{
    public function __construct(){}
}

$o = new ChildParentConstructorNotCalled(new Cryptopp\HashSha1());

try {
    $o->getDigestSize();
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
- empty key:
empty key ok
- no key:
no key ok
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
