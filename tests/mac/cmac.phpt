--TEST--
MAC algorithm: CMAC
--FILE--
<?php

var_dump(is_a("Cryptopp\MacCmac", "Cryptopp\MacAbstract", true));

// check algorithm infos
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\MacCmac($c);
var_dump($o->getName());
var_dump($o->getDigestSize());
var_dump($o->getBlockSize());

// key length check
echo "- key length check:\n";
var_dump($o->isValidKeyLength(32));
var_dump($o->isValidKeyLength(24));
var_dump($o->isValidKeyLength(16));
var_dump($o->isValidKeyLength(23));
var_dump($o->isValidKeyLength(125));
var_dump($o->isValidKeyLength(0));

// set key
echo "- set key:\n";
$o->setKey("1234567890123456");
var_dump($o->getKey());

// check digest calculation
echo "- digest calculation:\n";
$o->setKey(Cryptopp\HexUtils::hex2bin("2b7e151628aed2a6abf7158809cf4f3c"));
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

// check values returned by Cryptopp\Mac for this algorithm
echo "- Cryptopp\Mac:\n";
var_dump(in_array("cmac", Cryptopp\Mac::getAlgos()));
var_dump(Cryptopp\Mac::getClassname("cmac"));

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485760);
var_dump(strlen($o->calculateDigest($data)));
$o->update($data);
var_dump(strlen($o->finalize()));

?>
--EXPECT--
bool(true)
string(9) "cmac(aes)"
int(16)
int(0)
- key length check:
bool(true)
bool(true)
bool(true)
bool(false)
bool(false)
bool(false)
- set key:
string(16) "1234567890123456"
- digest calculation:
string(32) "caa7624159a7b2f383509739843c8f3f"
string(32) "6cc65b89ebbfbbb933a0db79d8c5f629"
- incremental hash:
string(32) "caa7624159a7b2f383509739843c8f3f"
- restart not necessary:
string(32) "caa7624159a7b2f383509739843c8f3f"
- restart:
string(32) "b19cb4f3c5ae573a086802d099c52f59"
- Cryptopp\Mac:
bool(true)
string(16) "Cryptopp\MacCmac"
- large data:
int(16)
int(16)
