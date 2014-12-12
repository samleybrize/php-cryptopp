--TEST--
Block cipher: AES
--FILE--
<?php

var_dump(is_a("Cryptopp\BlockCipherAes", "Cryptopp\BlockCipherAbstract", true));

// check that the class can be extended
class CipherChild extends Cryptopp\BlockCipherAes{}
$o = new CipherChild();
var_dump($o->getName());

// check algorithm infos
$o = new Cryptopp\BlockCipherAes();
var_dump($o->getName());
var_dump($o->getBlockSize());

// encrypt block > block size
$o->setKey("1234567890123456");
echo "- encrypt block != block size:\n";
try {
    $o->encryptBlock("123456789");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// decrypt block > block size
echo "- decrypt block != block size:\n";
try {
    $o->decryptBlock("123456789");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt not a multiple of block size
echo "- encrypt not a multiple of block size:\n";
try {
    $o->encrypt("12345678901234567");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// decrypt not a multiple of block size
echo "- decrypt not a multiple of block size:\n";
try {
    $o->decrypt("12345678901234567");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

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

// TODO encrypt without key

// sleep
echo "- sleep:\n";
try {
    serialize($o);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// test inheritance
echo "- inheritance:\n";
class Child extends Cryptopp\BlockCipherAes{}

$o = new Child();
var_dump($o->getBlockSize());

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\BlockCipherAes
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
string(3) "aes"
string(3) "aes"
int(16)
- encrypt block != block size:
Cryptopp\BlockCipherAes: data size (9) is not equal to cipher block size (16)
- decrypt block != block size:
Cryptopp\BlockCipherAes: data size (9) is not equal to cipher block size (16)
- encrypt not a multiple of block size:
Cryptopp\BlockCipherAes: data size (17) is not a multiple of block size (16)
- decrypt not a multiple of block size:
Cryptopp\BlockCipherAes: data size (17) is not a multiple of block size (16)
- invalid key:
Cryptopp\BlockCipherAes : 3 is not a valid key length
Cryptopp\BlockCipherAes : a key is required
- sleep:
You cannot serialize or unserialize Cryptopp\BlockCipherAbstract instances
- inheritance:
int(16)
- inheritance (parent constructor not called):
Cryptopp\BlockCipherAbstract cannot be extended by user classes
