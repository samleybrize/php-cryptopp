--TEST--
Authenticated symmetric cipher: GCM
--FILE--
<?php

var_dump(is_a("Cryptopp\AuthenticatedSymmetricCipherGcm", "Cryptopp\AuthenticatedSymmetricCipherAbstract", true));

// check algorithm infos
$o = new Cryptopp\AuthenticatedSymmetricCipherGcm(new Cryptopp\BlockCipherAes());
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->getDigestSize());

// encrypt
echo "- encrypt:\n";
$o->setKey(hex2bin("feffe9928665731c6d6a8f9467308308"));
$o->setIv(hex2bin("cafebabefacedbaddecaf888"));
var_dump(bin2hex($o->encrypt(hex2bin("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"))));
var_dump(bin2hex($o->encrypt(hex2bin("1c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b391aafd255"))));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt
echo "- decrypt:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e"))));
var_dump(bin2hex($o->decrypt(hex2bin("21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091473f5985"))));
var_dump(bin2hex($o->finalizeDecryption()));

// restart
echo "- restart:\n";
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"))));
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"))));

// encrypt data + aad
echo "- encrypt + aad:\n";
$o->restart();
$o->addEncryptionAdditionalData(hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2"));
$o->encrypt(hex2bin("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"));
$o->encrypt(hex2bin("1c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39"));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt data + aad
echo "- decrypt + aad:\n";
$o->restart();
$o->addDecryptionAdditionalData(hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2"));
$o->decrypt(hex2bin("42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e"));
$o->decrypt(hex2bin("21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091"));
var_dump(bin2hex($o->finalizeDecryption()));

// encrypt aad only
echo "- encrypt aad only:\n";
$o->setKey(hex2bin("77be63708971c4e240d1cb79e8d77feb"));
$o->setIv(hex2bin("e0e00f19fed7ba0136a797f3"));
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
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without key
echo "- no key:\n";
$o = new Cryptopp\AuthenticatedSymmetricCipherGcm(new Cryptopp\BlockCipherAes());

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// invalid iv
echo "- invalid iv:\n";
try {
    $o->setIv("123");
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

// TODO invalid cipher block size -> gcm with user class

// test inheritance
echo "- inheritance:\n";
class Child extends Cryptopp\AuthenticatedSymmetricCipherGcm{}

$o = new Child(new Cryptopp\BlockCipherAes());
var_dump($o->getBlockSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\AuthenticatedSymmetricCipherGcm
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
string(8) "gcm(aes)"
int(1)
int(16)
- encrypt:
string(64) "42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e"
string(64) "21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091473f5985"
string(32) "4d5c2af327cd64a62cf35abd2ba6fab4"
- decrypt:
string(64) "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"
string(64) "1c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b391aafd255"
string(32) "4d5c2af327cd64a62cf35abd2ba6fab4"
- restart:
string(64) "42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e"
string(64) "42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e"
- encrypt + aad:
string(32) "5bc94fbc3221a5db94fae95ae7121a47"
- decrypt + aad:
string(32) "5bc94fbc3221a5db94fae95ae7121a47"
- encrypt aad only:
string(32) "209fcc8d3675ed938e9c7166709dd946"
- decrypt aad only:
string(32) "209fcc8d3675ed938e9c7166709dd946"
- invalid key:
Cryptopp\AuthenticatedSymmetricCipherGcm : 33 is not a valid key length
Cryptopp\AuthenticatedSymmetricCipherGcm : a key is required
- no key:
Cryptopp\AuthenticatedSymmetricCipherGcm : a key is required
- invalid iv:
Cryptopp\AuthenticatedSymmetricCipherGcm : 3 is not a valid initialization vector length
- sleep:
You cannot serialize or unserialize Cryptopp\AuthenticatedSymmetricCipherAbstract instances
- inheritance:
int(1)
- inheritance (parent constructor not called):
Cryptopp\AuthenticatedSymmetricCipherAbstract cannot be extended by user classes
