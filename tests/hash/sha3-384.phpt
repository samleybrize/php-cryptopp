--TEST--
Hash algorithm: SHA3-384
--SKIPIF--
<?php
if (!class_exists("Cryptopp\HashSha3_384")) {
    echo "skip sha3 not available";
}
?>
--FILE--
<?php

var_dump(is_a("Cryptopp\HashSha3_384", "Cryptopp\HashAbstract", true));

// check algorithm infos
$o = new Cryptopp\HashSha3_384();
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
var_dump(in_array("sha3_384", Cryptopp\Hash::getAlgos()));
var_dump(Cryptopp\Hash::getClassname("sha3_384"));

// sleep
echo "- sleep:\n";
try {
    serialize($o);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// test inheritance
echo "- inheritance:\n";
class Child extends Cryptopp\HashSha3_384{}

$o = new Child();
var_dump($o->getBlockSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\HashSha3_384
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
string(8) "sha3_384"
int(136)
int(48)
- digest calculation:
string(96) "979f984c6de6d69d4137c8a92d4c2509a7adcc4d40e9880f761dcc45498408fa475ab56f656fc9f41b05d5f0c42bb92a"
string(96) "e27d047d91207046d60f405ced5d0f177d81374b6cfa7851a81966f36434e4b1c65b8e1eb6dc7965b782abdfba986bea"
- incremental hash:
string(96) "979f984c6de6d69d4137c8a92d4c2509a7adcc4d40e9880f761dcc45498408fa475ab56f656fc9f41b05d5f0c42bb92a"
- restart not necessary:
string(96) "979f984c6de6d69d4137c8a92d4c2509a7adcc4d40e9880f761dcc45498408fa475ab56f656fc9f41b05d5f0c42bb92a"
- restart:
string(96) "cc506f90bb6bc68d5553f0293d9e9b6ba61113c95cf2f782eac04a33c5aa213d8849df42b74892ba3b34bf070594c32f"
- Cryptopp\Hash:
bool(true)
string(21) "Cryptopp\HashSha3_384"
- sleep:
You cannot serialize or unserialize Cryptopp\HashAbstract instances
- inheritance:
int(136)
- inheritance (parent constructor not called):
Cryptopp\HashAbstract cannot be extended by user classes
