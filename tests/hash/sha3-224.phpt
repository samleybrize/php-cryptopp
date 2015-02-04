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
var_dump(Cryptopp\HexUtils::bin2hex($o->calculateDigest("qwertyuiop")));
var_dump(Cryptopp\HexUtils::bin2hex($o->calculateDigest("azerty")));

// check incremental hash
echo "- incremental hash:\n";
$o->update("qwerty");
$o->update("uio");
$o->update("p");
var_dump(Cryptopp\HexUtils::bin2hex($o->finalize()));

// check that a restart() is not necessary after a call to finalize()
echo "- restart not necessary:\n";
$o->update("qwerty");
$o->update("uio");
$o->update("p");
var_dump(Cryptopp\HexUtils::bin2hex($o->finalize()));

// check restart()
echo "- restart:\n";
$o->update("qwerty");
$o->restart();
$o->update("uio");
$o->update("p");
var_dump(Cryptopp\HexUtils::bin2hex($o->finalize()));

// check values returned by Cryptopp\Hash for this algorithm
echo "- Cryptopp\Hash:\n";
var_dump(in_array("sha3_224", Cryptopp\Hash::getAlgos()));
var_dump(Cryptopp\Hash::getClassname("sha3_224"));

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485760);
var_dump(strlen($o->calculateDigest($data)));
$o->update($data);
var_dump(strlen($o->finalize()));

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
- large data:
int(28)
int(28)
