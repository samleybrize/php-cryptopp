--TEST--
Hash algorithm: SHA1
--FILE--
<?php

var_dump(is_a("Cryptopp\HashSha1", "Cryptopp\HashAbstract", true));

// check that the class can be extended
class HashChild extends Cryptopp\HashSha1{}
$o = new HashChild();
var_dump($o->getName());

// check algorithm infos
$o = new Cryptopp\HashSha1();
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->getDigestSize());

// check digest calculation
echo "\n";
var_dump(bin2hex($o->calculateDigest("qwertyuiop")));
var_dump(bin2hex($o->calculateDigest("azerty")));

// check incremental hash
echo "\n";
$o->update("qwerty");
$o->update("uio");
$o->update("p");
var_dump(bin2hex($o->finalize()));

// check that a restart() is not necessary after a call to finalize()
echo "\n";
$o->update("qwerty");
$o->update("uio");
$o->update("p");
var_dump(bin2hex($o->finalize()));

// check restart()
echo "\n";
$o->update("qwerty");
$o->restart();
$o->update("uio");
$o->update("p");
var_dump(bin2hex($o->finalize()));

// check values returned by Cryptopp\Hash for this algorithm
echo "\n";
var_dump(in_array("sha1", Cryptopp\Hash::getAlgos()));
var_dump(Cryptopp\Hash::getClassname("sha1"));

// test inheritance
echo "\n";
class Child extends Cryptopp\HashSha1{}

$o = new Child();
var_dump($o->getBlockSize());

// test inheritance - parent constructor not called
echo "\n";
class ChildParentConstructorNotCalled extends Cryptopp\HashSha1
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
string(4) "sha1"
string(4) "sha1"
int(64)
int(20)

string(40) "b0399d2029f64d445bd131ffaa399a42d2f8e7dc"
string(40) "9cf95dacd226dcf43da376cdb6cbba7035218921"

string(40) "b0399d2029f64d445bd131ffaa399a42d2f8e7dc"

string(40) "b0399d2029f64d445bd131ffaa399a42d2f8e7dc"

string(40) "3334ae6608f4ed4b9da5a946aff43f4dabc3e6b6"

bool(true)
string(17) "Cryptopp\HashSha1"

int(64)

Cryptopp\HashAbstract cannot be extended by user classes
