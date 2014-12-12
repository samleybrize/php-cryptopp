--TEST--
MAC algorithm: Two-Track-Mac
--FILE--
<?php

var_dump(is_a("Cryptopp\MacTwoTrackMac", "Cryptopp\MacAbstract", true));

// check that the class can be extended
class MacChild extends Cryptopp\MacTwoTrackMac{}
$o = new MacChild();
var_dump($o->getName());

// check algorithm infos
$o = new Cryptopp\MacTwoTrackMac();
var_dump($o->getName());
var_dump($o->getDigestSize());

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

// check verify()
echo "- verify:\n";
var_dump($o->verify(hex2bin("e64c1b6d1e1b062b57bafabe75816a121c2f7b34"), hex2bin("e64c1b6d1e1b062b57bafabe75816a121c2f7b34")));
var_dump($o->verify(hex2bin("e64c1b6d1e1b062b57bafabe75816a121c2f7b34"), hex2bin("b27165d2f7de41c23aabe559cad7cc592fb50194")));
var_dump($o->verify(hex2bin("e64c1b6d1e1b062b57bafabe75816a121c2f7b34"), hex2bin("e64c1b6d1e1b062b57bafabe75816a121c2f7b33")));

// check values returned by Cryptopp\Mac for this algorithm
echo "- Cryptopp\Mac:\n";
var_dump(in_array("two_track_mac", Cryptopp\Mac::getAlgos()));
var_dump(Cryptopp\Mac::getClassname("two_track_mac"));

// invalid key
echo "- invalid key:\n";
try {
    $o->setKey("145");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $o->setKey("");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without key
echo "- no key:\n";
$o = new Cryptopp\MacTwoTrackMac();

try {
    $o->calculateDigest("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// sleep
echo "- sleep:\n";
try {
    serialize($o);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// test inheritance
echo "- inheritance:\n";
class Child extends Cryptopp\MacTwoTrackMac{}

$o = new Child();
var_dump($o->getDigestSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\MacTwoTrackMac
{
    public function __construct(){}
}

$o = new ChildParentConstructorNotCalled();

try {
    $o->getDigestSize();
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
bool(true)
string(13) "two_track_mac"
string(13) "two_track_mac"
int(20)
- digest calculation:
string(40) "e64c1b6d1e1b062b57bafabe75816a121c2f7b34"
string(40) "b27165d2f7de41c23aabe559cad7cc592fb50194"
- incremental hash:
string(40) "e64c1b6d1e1b062b57bafabe75816a121c2f7b34"
- restart not necessary:
string(40) "e64c1b6d1e1b062b57bafabe75816a121c2f7b34"
- restart:
string(40) "b3bcefdbee9284259388c565fc4a16a6d7304709"
- verify:
bool(true)
bool(false)
bool(false)
- Cryptopp\Mac:
bool(true)
string(23) "Cryptopp\MacTwoTrackMac"
- invalid key:
Cryptopp\MacTwoTrackMac : 3 is not a valid key length
Cryptopp\MacTwoTrackMac : a key is required
- no key:
Cryptopp\MacTwoTrackMac : a key is required
- sleep:
You cannot serialize or unserialize Cryptopp\MacAbstract instances
- inheritance:
int(20)
- inheritance (parent constructor not called):
Cryptopp\MacAbstract cannot be extended by user classes
