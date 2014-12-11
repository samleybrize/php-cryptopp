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

// check that the class can be extended
class HashChild extends Cryptopp\HashSha3_224{}
$o = new HashChild();
var_dump($o->getName());

// check algorithm infos
$o = new Cryptopp\HashSha3_224();
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->getDigestSize());

// check digest calculation
var_dump(bin2hex($o->calculateDigest("qwertyuiop")));
var_dump(bin2hex($o->calculateDigest("azerty")));

// check incremental hash
$o->update("qwerty");
$o->update("uio");
$o->update("p");
var_dump(bin2hex($o->finalize()));

// check that a restart() is not necessary after a call to finalize()
$o->update("qwerty");
$o->update("uio");
$o->update("p");
var_dump(bin2hex($o->finalize()));

// check restart()
$o->update("qwerty");
$o->restart();
$o->update("uio");
$o->update("p");
var_dump(bin2hex($o->finalize()));

?>
--EXPECT--
bool(true)
string(8) "sha3_224"
string(8) "sha3_224"
int(0)
int(28)
string(56) "2fa05a669e02a13295588c05a1e91b56d889cf5004f9971789a464bf"
string(56) "39ba050e26e31d0b3e1293a33dfbdecba37e2f0a6e851dd4bed8ccfc"
string(56) "2fa05a669e02a13295588c05a1e91b56d889cf5004f9971789a464bf"
string(56) "2fa05a669e02a13295588c05a1e91b56d889cf5004f9971789a464bf"
string(56) "bcd289219436dabe3b03048bd00913dfe26e86a80a55137f50fc569b"
