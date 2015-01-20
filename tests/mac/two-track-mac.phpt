--TEST--
MAC algorithm: Two-Track-Mac
--FILE--
<?php

var_dump(is_a("Cryptopp\MacTwoTrackMac", "Cryptopp\MacAbstract", true));

// check algorithm infos
$o = new Cryptopp\MacTwoTrackMac();
var_dump($o->getName());
var_dump($o->getDigestSize());
var_dump($o->getBlockSize());

// key length check
echo "- key length check:\n";
var_dump($o->isValidKeyLength(20));
var_dump($o->isValidKeyLength(19));
var_dump($o->isValidKeyLength(21));

// set key
echo "- set key:\n";
$o->setKey("azertyuiopqsdfghjklm");
var_dump($o->getKey());

// check digest calculation
echo "- digest calculation:\n";
$o->setKey(hex2bin("00112233445566778899aabbccddeeff01234567"));
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

// check values returned by Cryptopp\Mac for this algorithm
echo "- Cryptopp\Mac:\n";
var_dump(in_array("two_track_mac", Cryptopp\Mac::getAlgos()));
var_dump(Cryptopp\Mac::getClassname("two_track_mac"));

?>
--EXPECT--
bool(true)
string(13) "two_track_mac"
int(20)
int(64)
- key length check:
bool(true)
bool(false)
bool(false)
- set key:
string(20) "azertyuiopqsdfghjklm"
- digest calculation:
string(40) "e64c1b6d1e1b062b57bafabe75816a121c2f7b34"
string(40) "b27165d2f7de41c23aabe559cad7cc592fb50194"
- incremental hash:
string(40) "e64c1b6d1e1b062b57bafabe75816a121c2f7b34"
- restart not necessary:
string(40) "e64c1b6d1e1b062b57bafabe75816a121c2f7b34"
- restart:
string(40) "b3bcefdbee9284259388c565fc4a16a6d7304709"
- Cryptopp\Mac:
bool(true)
string(23) "Cryptopp\MacTwoTrackMac"
