--TEST--
MAC algorithm: HMAC
--FILE--
<?php

var_dump(is_a("Cryptopp\MacHmac", "Cryptopp\MacAbstract", true));

// check algorithm infos
$o = new Cryptopp\MacHmac(new Cryptopp\HashMd5());
var_dump($o->getName());
var_dump($o->getDigestSize());
var_dump($o->getBlockSize());

$o = new Cryptopp\MacHmac(new Cryptopp\HashSha1());
var_dump($o->getName());
var_dump($o->getDigestSize());
var_dump($o->getBlockSize());

// check digest calculation
echo "- digest calculation (sha1):\n";
$o->setKey(hex2bin("0102030405060708090a0b0c0d0e0f10111213141516171819"));
var_dump(bin2hex($o->calculateDigest("qwertyuiop")));
var_dump(bin2hex($o->calculateDigest("azerty")));

// check incremental hash
echo "- incremental hash:\n";
$o->update("qwerty");
$o->update("uio");
$o->update("p");
var_dump(bin2hex($o->finalize()));

// check that a restart() is not necessary after a call to finalize()
echo "- restart not necessary:\n";
$o->update("qwerty");
$o->update("uio");
$o->update("p");
var_dump(bin2hex($o->finalize()));

// check restart()
echo "- restart:\n";
$o->update("qwerty");
$o->restart();
$o->update("uio");
$o->update("p");
var_dump(bin2hex($o->finalize()));

// check different hash algorithm
echo "- calculate digest (md5):\n";
$o = new Cryptopp\MacHmac(new Cryptopp\HashMd5());
$o->setKey(hex2bin("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b"));
var_dump(bin2hex($o->calculateDigest("Hi There")));

// check values returned by Cryptopp\Mac for this algorithm
echo "- Cryptopp\Mac:\n";
var_dump(in_array("hmac", Cryptopp\Mac::getAlgos()));
var_dump(Cryptopp\Mac::getClassname("hmac"));

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
bool(true)
string(9) "hmac(md5)"
int(16)
int(0)
string(10) "hmac(sha1)"
int(20)
int(0)
- digest calculation (sha1):
string(40) "b7b39196ab5f9c0cf7863b8e0a0bda37aea2c93e"
string(40) "286d11632a144649124bf912f2826ee80887206f"
- incremental hash:
string(40) "b7b39196ab5f9c0cf7863b8e0a0bda37aea2c93e"
- restart not necessary:
string(40) "b7b39196ab5f9c0cf7863b8e0a0bda37aea2c93e"
- restart:
string(40) "7ad59cd33a3a5657638de193a69d68c91019701a"
- calculate digest (md5):
string(32) "9294727a3638bb1c13f48ef8158bfc9d"
- Cryptopp\Mac:
bool(true)
string(16) "Cryptopp\MacHmac"
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
