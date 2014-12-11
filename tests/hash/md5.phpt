--TEST--
Hash algorithm: MD5
--FILE--
<?php

var_dump(is_a("Cryptopp\HashMd5", "Cryptopp\HashAbstract", true));

// check that the class can be extended
class HashChild extends Cryptopp\HashMd5{}
$o = new HashChild();
var_dump($o->getName());

// check algorithm infos
$o = new Cryptopp\HashMd5();
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

// check values returned by Cryptopp\Hash for this algorithm
var_dump(in_array("md5", Cryptopp\Hash::getAlgos()));
var_dump(Cryptopp\Hash::getClassname("md5"));

?>
--EXPECT--
bool(true)
string(3) "md5"
string(3) "md5"
int(64)
int(16)
string(32) "6eea9b7ef19179a06954edd0f6c05ceb"
string(32) "ab4f63f9ac65152575886860dde480a1"
string(32) "6eea9b7ef19179a06954edd0f6c05ceb"
string(32) "6eea9b7ef19179a06954edd0f6c05ceb"
string(32) "ba266745410d3c888ad3ca53f55e3b4f"
bool(true)
string(16) "Cryptopp\HashMd5"
