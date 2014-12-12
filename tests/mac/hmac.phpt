--TEST--
MAC algorithm: HMAC
--FILE--
<?php

var_dump(is_a("Cryptopp\MacHmac", "Cryptopp\MacAbstract", true));

// check that the class can be extended
class MacChild extends Cryptopp\MacHmac{}
$o = new MacChild(new Cryptopp\HashSha1());
var_dump($o->getName());

// check algorithm infos
$o = new Cryptopp\MacHmac(new Cryptopp\HashMd5());
var_dump($o->getName());
var_dump($o->getDigestSize());

$o = new Cryptopp\MacHmac(new Cryptopp\HashSha1());
var_dump($o->getName());
var_dump($o->getDigestSize());

// check digest calculation
echo "\n";
$o->setKey(hex2bin("0102030405060708090a0b0c0d0e0f10111213141516171819"));
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

// check verify()
echo "\n";
var_dump($o->verify(hex2bin("e64c1b6d1e1b062b57bafabe75816a121c2f7b34"), hex2bin("e64c1b6d1e1b062b57bafabe75816a121c2f7b34")));
var_dump($o->verify(hex2bin("e64c1b6d1e1b062b57bafabe75816a121c2f7b34"), hex2bin("b27165d2f7de41c23aabe559cad7cc592fb50194")));
var_dump($o->verify(hex2bin("e64c1b6d1e1b062b57bafabe75816a121c2f7b34"), hex2bin("e64c1b6d1e1b062b57bafabe75816a121c2f7b33")));

// check different hash algorithm
echo "\n";
$o = new Cryptopp\MacHmac(new Cryptopp\HashMd5());
$o->setKey(hex2bin("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b"));
var_dump(bin2hex($o->calculateDigest("Hi There")));

// check values returned by Cryptopp\Mac for this algorithm
echo "\n";
var_dump(in_array("hmac", Cryptopp\Mac::getAlgos()));
var_dump(Cryptopp\Mac::getClassname("hmac"));

// invalid key
echo "\n";
try {
    $o->setKey("");
    echo "key ok\n";
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// sleep
echo "\n";
try {
    serialize($o);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// test inheritance
echo "\n";
class Child extends Cryptopp\MacHmac{}

$o = new Child(new Cryptopp\HashSha1());
var_dump($o->getDigestSize());

// test inheritance - parent constructor not called
echo "\n";
class ChildParentConstructorNotCalled extends Cryptopp\MacHmac
{
    public function __construct(){}
}

$o = new ChildParentConstructorNotCalled(new Cryptopp\HashSha1());

try {
    $o->getDigestSize();
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
bool(true)
string(10) "hmac(sha1)"
string(9) "hmac(md5)"
int(16)
string(10) "hmac(sha1)"
int(20)

string(40) "b7b39196ab5f9c0cf7863b8e0a0bda37aea2c93e"
string(40) "286d11632a144649124bf912f2826ee80887206f"

string(40) "b7b39196ab5f9c0cf7863b8e0a0bda37aea2c93e"

string(40) "b7b39196ab5f9c0cf7863b8e0a0bda37aea2c93e"

string(40) "7ad59cd33a3a5657638de193a69d68c91019701a"

bool(true)
bool(false)
bool(false)

string(32) "9294727a3638bb1c13f48ef8158bfc9d"

bool(true)
string(16) "Cryptopp\MacHmac"

key ok

You cannot serialize or unserialize Cryptopp\MacAbstract instances

int(20)

Cryptopp\MacAbstract cannot be extended by user classes
