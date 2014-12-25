--TEST--
Hash algorithm: SHA3-224
--SKIPIF--
<?php
if (!class_exists("Cryptopp\HashSha3_224")) {
    echo "skip sha3 not available";
}
?>
--FILE--
<?php

var_dump(is_a("Cryptopp\HashSha3_224", "Cryptopp\HashAbstract", true));

// check algorithm infos
$o = new Cryptopp\HashSha3_224();
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->getDigestSize());

// check digest calculation
echo "- digest calculation:\n";
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

// check values returned by Cryptopp\Hash for this algorithm
echo "- Cryptopp\Hash:\n";
var_dump(in_array("sha3_224", Cryptopp\Hash::getAlgos()));
var_dump(Cryptopp\Hash::getClassname("sha3_224"));

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

// test inheritance
echo "- inheritance:\n";
class Child extends Cryptopp\HashSha3_224{}

$o = new Child();
var_dump($o->getBlockSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\HashSha3_224
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
string(8) "sha3_224"
int(72)
int(28)
- digest calculation:
string(56) "2fa05a669e02a13295588c05a1e91b56d889cf5004f9971789a464bf"
string(56) "39ba050e26e31d0b3e1293a33dfbdecba37e2f0a6e851dd4bed8ccfc"
- incremental hash:
string(56) "2fa05a669e02a13295588c05a1e91b56d889cf5004f9971789a464bf"
- restart not necessary:
string(56) "2fa05a669e02a13295588c05a1e91b56d889cf5004f9971789a464bf"
- restart:
string(56) "bcd289219436dabe3b03048bd00913dfe26e86a80a55137f50fc569b"
- Cryptopp\Hash:
bool(true)
string(21) "Cryptopp\HashSha3_224"
- sleep:
You cannot serialize or unserialize Cryptopp\HashAbstract instances
- bad arguments:
NULL
Cryptopp\HashAbstract::calculateDigest() expects parameter 1 to be string, array given
NULL
Cryptopp\HashAbstract::update() expects parameter 1 to be string, array given
- inheritance:
int(72)
- inheritance (parent constructor not called):
Cryptopp\HashAbstract cannot be extended by user classes
