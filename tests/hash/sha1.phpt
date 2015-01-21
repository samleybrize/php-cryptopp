--TEST--
Hash algorithm: SHA1
--FILE--
<?php

var_dump(is_a("Cryptopp\HashSha1", "Cryptopp\HashAbstract", true));

// check algorithm infos
$o = new Cryptopp\HashSha1();
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
var_dump(in_array("sha1", Cryptopp\Hash::getAlgos()));
var_dump(Cryptopp\Hash::getClassname("sha1"));

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485760);
var_dump(strlen($o->calculateDigest($data)));
$o->update($data);
var_dump(strlen($o->finalize()));

?>
--EXPECT--
bool(true)
string(4) "sha1"
int(64)
int(20)
- digest calculation:
string(40) "b0399d2029f64d445bd131ffaa399a42d2f8e7dc"
string(40) "9cf95dacd226dcf43da376cdb6cbba7035218921"
- incremental hash:
string(40) "b0399d2029f64d445bd131ffaa399a42d2f8e7dc"
- restart not necessary:
string(40) "b0399d2029f64d445bd131ffaa399a42d2f8e7dc"
- restart:
string(40) "3334ae6608f4ed4b9da5a946aff43f4dabc3e6b6"
- Cryptopp\Hash:
bool(true)
string(17) "Cryptopp\HashSha1"
- large data:
int(20)
int(20)
