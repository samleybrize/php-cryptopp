--TEST--
Authenticated symmetric cipher generic
--SKIPIF--
<?php die("disabled"); ?>
--FILE--
<?php

var_dump(is_a("Cryptopp\AuthenticatedSymmetricCipherGeneric", "Cryptopp\AuthenticatedSymmetricCipherAbstract", true));

// TODO setKey
// TODO setIv

// TODO getCipher
// TODO getMac

// TODO test cipher w/o iv

// check algorithm infos
$c = new Cryptopp\StreamCipherSosemanuk();
$m = new Cryptopp\MacHmac(new Cryptopp\HashSha1());
$o = new Cryptopp\AuthenticatedSymmetricCipherGeneric($c, $m);
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->getDigestSize());

// encrypt
echo "- encrypt:\n";
$m->setKey(hex2bin("feffe9928665731c6d6a8f9467308308"));
$c->setKey(hex2bin("a7c083feb7"));
$c->setIv(hex2bin("00112233445566778899aabbccddeeff")); // TODO $c
var_dump(bin2hex($o->encrypt(hex2bin("00000000000000000000000000000000"))));
var_dump(bin2hex($o->encrypt(hex2bin("00000000000000000000000000000000"))));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt
echo "- decrypt:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("fe81d2162c9a100d04895c454a77515b"))));
var_dump(bin2hex($o->decrypt(hex2bin("be6a431a935cb90e2221ebb7ef502328"))));
var_dump(bin2hex($o->finalizeDecryption()));

// restart
echo "- restart:\n";
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("00000000000000000000000000000000"))));
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("00000000000000000000000000000000"))));
var_dump(bin2hex($o->finalizeEncryption()));

// encrypt data + aad
echo "- encrypt + aad:\n";
$o->restart();
$o->addEncryptionAdditionalData(hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2"));
$o->encrypt(hex2bin("00000000000000000000000000000000"));
$o->encrypt(hex2bin("00000000000000000000000000000000"));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt data + aad
echo "- decrypt + aad:\n";
$o->restart();
$o->addDecryptionAdditionalData(hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2"));
$o->decrypt(hex2bin("fe81d2162c9a100d04895c454a77515b"));
$o->decrypt(hex2bin("be6a431a935cb90e2221ebb7ef502328"));
var_dump(bin2hex($o->finalizeDecryption()));

// encrypt aad only
echo "- encrypt aad only:\n";
$c->setKey(hex2bin("77be637089"));
$c->setIv(hex2bin("e0e00f19fed7ba0136a797f3ed7ba013"));
$o->addEncryptionAdditionalData(hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt aad only
echo "- decrypt aad only:\n";
$o->restart();
$o->addDecryptionAdditionalData(hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"));
var_dump(bin2hex($o->finalizeDecryption()));

// invalid key
echo "- invalid key:\n";
try {
    $o->setKey("123456789012345678901234567890123");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $o->setKey("");
    echo "empty key ok\n";
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without key
echo "- no key:\n";
$c = new Cryptopp\StreamCipherSosemanuk();
$m = new Cryptopp\MacHmac(new Cryptopp\HashSha1());
$o = new Cryptopp\AuthenticatedSymmetricCipherGeneric($c, $m);

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// invalid iv
echo "- invalid iv:\n";
try {
    $o->setIv("123");
    echo "iv size 3 ok\n";
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt before aad
echo "- encrypt before aad:\n";
$o->restart();
$o->setKey("1234567890123456");
$o->setIv("1234567890123456");

$o->encrypt("azerty");

try {
    $o->addEncryptionAdditionalData("qwerty");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// decrypt before aad
echo "- decrypt before aad:\n";
$o->decrypt("azerty");

try {
    $o->addDecryptionAdditionalData("qwerty");
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

// bad arguments
echo "- bad arguments:\n";
var_dump(@$o->encrypt(array()));
echo "$php_errormsg\n";
var_dump(@$o->decrypt(array()));
echo "$php_errormsg\n";
var_dump(@$o->addEncryptionAdditionalData(array()));
echo "$php_errormsg\n";
var_dump(@$o->addDecryptionAdditionalData(array()));
echo "$php_errormsg\n";
var_dump(@$o->setKey(array()));
echo "$php_errormsg\n";
var_dump(@$o->setIv(array()));
echo "$php_errormsg\n";

// test inheritance
echo "- inheritance:\n";
class Child extends Cryptopp\AuthenticatedSymmetricCipherGeneric{}

$o = new Child($c, $m);
var_dump($o->getBlockSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\AuthenticatedSymmetricCipherGeneric
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
string(20) "sosemanuk/hmac(sha1)"
int(1)
int(20)
- encrypt:
string(32) "fe81d2162c9a100d04895c454a77515b"
string(32) "be6a431a935cb90e2221ebb7ef502328"
string(40) "15cbfe20bb447711c2700b5eddada57323007973"
- decrypt:
string(32) "00000000000000000000000000000000"
string(32) "00000000000000000000000000000000"
string(40) "15cbfe20bb447711c2700b5eddada57323007973"
- restart:
string(32) "fe81d2162c9a100d04895c454a77515b"
string(32) "fe81d2162c9a100d04895c454a77515b"
string(40) "16bb821c7ba92cc135b931716b30410d13869fa8"
- encrypt + aad:
string(40) "ccc6f4a1ccb0c3e03a0b3e103613bef65d5f61de"
- decrypt + aad:
string(40) "ccc6f4a1ccb0c3e03a0b3e103613bef65d5f61de"
- encrypt aad only:
string(40) "02a2913e1a34d07005ebf2ba59a1008ba1f1307f"
- decrypt aad only:
string(40) "02a2913e1a34d07005ebf2ba59a1008ba1f1307f"
- invalid key:
Cryptopp\AuthenticatedSymmetricCipherGeneric : 33 is not a valid key length
empty key ok
- no key:
Cryptopp\AuthenticatedSymmetricCipherGeneric : a key is required
- invalid iv:
iv size 3 ok
- encrypt before aad:
Cryptopp\AuthenticatedSymmetricCipherGeneric: additional authenticated data must be added before any encryption
- decrypt before aad:
Cryptopp\AuthenticatedSymmetricCipherGeneric: additional authenticated data must be added before any decryption
- sleep:
You cannot serialize or unserialize Cryptopp\AuthenticatedSymmetricCipherAbstract instances
- bad arguments:
NULL
Cryptopp\AuthenticatedSymmetricCipherAbstract::encrypt() expects parameter 1 to be string, array given
NULL
Cryptopp\AuthenticatedSymmetricCipherAbstract::decrypt() expects parameter 1 to be string, array given
NULL
Cryptopp\AuthenticatedSymmetricCipherAbstract::addEncryptionAdditionalData() expects parameter 1 to be string, array given
NULL
Cryptopp\AuthenticatedSymmetricCipherAbstract::addDecryptionAdditionalData() expects parameter 1 to be string, array given
NULL
Cryptopp\AuthenticatedSymmetricCipherAbstract::setKey() expects parameter 1 to be string, array given
NULL
Cryptopp\AuthenticatedSymmetricCipherAbstract::setIv() expects parameter 1 to be string, array given
- inheritance:
int(1)
- inheritance (parent constructor not called):
Cryptopp\AuthenticatedSymmetricCipherAbstract cannot be extended by user classes
