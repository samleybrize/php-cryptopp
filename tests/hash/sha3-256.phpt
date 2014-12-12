--TEST--
Hash algorithm: SHA3-256
--SKIPIF--
<?php
if (!class_exists("Cryptopp\HashSha3_256")) {
    echo "skip sha3 not available";
}
?>
--FILE--
<?php

var_dump(is_a("Cryptopp\HashSha3_256", "Cryptopp\HashAbstract", true));

// check that the class can be extended
class HashChild extends Cryptopp\HashSha3_256{}
$o = new HashChild();
var_dump($o->getName());

// check algorithm infos
$o = new Cryptopp\HashSha3_256();
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
var_dump(in_array("sha3_256", Cryptopp\Hash::getAlgos()));
var_dump(Cryptopp\Hash::getClassname("sha3_256"));

// sleep
echo "\n";
try {
    serialize($o);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// test inheritance
echo "\n";
class Child extends Cryptopp\HashSha3_256{}

$o = new Child();
var_dump($o->getBlockSize());

// test inheritance - parent constructor not called
echo "\n";
class ChildParentConstructorNotCalled extends Cryptopp\HashSha3_256
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
string(8) "sha3_256"
string(8) "sha3_256"
int(0)
int(32)

string(64) "caeaa437035747dc5931abb3cd05c0121d02e21c31c1867d01288bc9295f1365"
string(64) "113b396bb409b4193c303272b695cbbf9da205466dd3688a71ddc2a3966e784e"

string(64) "caeaa437035747dc5931abb3cd05c0121d02e21c31c1867d01288bc9295f1365"

string(64) "caeaa437035747dc5931abb3cd05c0121d02e21c31c1867d01288bc9295f1365"

string(64) "8d56d47093eebcbfdac9d6c72a241ea5100a73ca7d1ffb42bfd8668abdbed863"

bool(true)
string(21) "Cryptopp\HashSha3_256"

You cannot serialize or unserialize Cryptopp\HashAbstract instances

int(0)

Cryptopp\HashAbstract cannot be extended by user classes
