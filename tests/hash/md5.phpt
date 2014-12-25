--TEST--
Hash algorithm: MD5
--FILE--
<?php

var_dump(is_a("Cryptopp\HashMd5", "Cryptopp\HashAbstract", true));

// check algorithm infos
$o = new Cryptopp\HashMd5();
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
var_dump(in_array("md5", Cryptopp\Hash::getAlgos()));
var_dump(Cryptopp\Hash::getClassname("md5"));

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
class Child extends Cryptopp\HashMd5{}

$o = new Child();
var_dump($o->getBlockSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\HashMd5
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
--EXPECTF--
bool(true)
string(3) "md5"
int(64)
int(16)
- digest calculation:
string(32) "6eea9b7ef19179a06954edd0f6c05ceb"
string(32) "ab4f63f9ac65152575886860dde480a1"
- incremental hash:
string(32) "6eea9b7ef19179a06954edd0f6c05ceb"
- restart not necessary:
string(32) "6eea9b7ef19179a06954edd0f6c05ceb"
- restart:
string(32) "ba266745410d3c888ad3ca53f55e3b4f"
- Cryptopp\Hash:
bool(true)
string(16) "Cryptopp\HashMd5"
- sleep:
You cannot serialize or unserialize Cryptopp\HashAbstract instances
- bad arguments:
NULL
Cryptopp\HashAbstract::calculateDigest() expects parameter 1 to be string, array given
NULL
Cryptopp\HashAbstract::update() expects parameter 1 to be string, array given
- inheritance:
int(64)
- inheritance (parent constructor not called):
Cryptopp\HashAbstract cannot be extended by user classes
