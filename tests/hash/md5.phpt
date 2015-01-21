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

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485760);
var_dump(strlen($o->calculateDigest($data)));
$o->update($data);
var_dump(strlen($o->finalize()));

?>
--EXPECT--
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
- large data:
int(16)
int(16)
