--TEST--
Symmetric transformation filter
--FILE--
<?php

$o = new Cryptopp\SymmetricTransformationFilter(new Cryptopp\StreamCipherSosemanuk());
var_dump($o->getCipher()->getName());

// check default padding
var_dump(get_class($o->getPadding()));

// check final methods
echo "- final methods:\n";
$reflection = new ReflectionClass("Cryptopp\SymmetricTransformationFilter");
$methods    = $reflection->getMethods();

foreach ($methods as $method) {
    if (!$method->isFinal()) {
        continue;
    }

    var_dump($method->getName());
}

// encryptString
echo "- encryptString:\n";
$m = new Cryptopp\SymmetricModeEcb(new Cryptopp\BlockCipherAes());
$o = new Cryptopp\SymmetricTransformationFilter($m, new Cryptopp\PaddingPkcs7());
$m->setKey("1234567890123456");
var_dump(bin2hex($o->encryptString("azertyuiop")));
var_dump(bin2hex($o->encryptString("azertyuiop")));
var_dump(bin2hex($o->encryptString("wxcvbnqsdfg")));

// decryptString
echo "- decryptString:\n";
var_dump($o->decryptString(hex2bin("ce86616e5e9d59edfc4ee2cc002f601b")));
var_dump($o->decryptString(hex2bin("ce86616e5e9d59edfc4ee2cc002f601b")));
var_dump($o->decryptString(hex2bin("da7885b5ad196250c037599a7bf9d62a")));

// sleep
echo "- sleep:\n";
try {
    serialize($o);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// mode object: constructor not called
echo "- mode object (parent constructor not called):\n";
class ModeChild extends Cryptopp\SymmetricModeEcb
{
    public function __construct(){}
}

try {
    $o = new Cryptopp\SymmetricTransformationFilter(new ModeChild());
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// stream object: constructor not called
echo "- stream cipher object (parent constructor not called):\n";
class StreamChild extends Cryptopp\StreamCipherSosemanuk
{
    public function __construct(){}
}

try {
    $o = new Cryptopp\SymmetricTransformationFilter(new StreamChild());
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\SymmetricTransformationFilter
{
    public function __construct(){}
}

$o = new ChildParentConstructorNotCalled();

try {
    $o->encryptString("");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $o->decryptString("");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
string(9) "sosemanuk"
string(21) "Cryptopp\PaddingPkcs7"
- final methods:
string(7) "__sleep"
string(8) "__wakeup"
string(9) "getCipher"
string(10) "getPadding"
string(13) "encryptString"
string(13) "decryptString"
- encryptString:
string(32) "ce86616e5e9d59edfc4ee2cc002f601b"
string(32) "ce86616e5e9d59edfc4ee2cc002f601b"
string(32) "da7885b5ad196250c037599a7bf9d62a"
- decryptString:
string(10) "azertyuiop"
string(10) "azertyuiop"
string(11) "wxcvbnqsdfg"
- sleep:
You cannot serialize or unserialize Cryptopp\SymmetricTransformationFilter instances
- mode object (parent constructor not called):
ModeChild : parent constructor was not called
- stream cipher object (parent constructor not called):
StreamChild : parent constructor was not called
- inheritance (parent constructor not called):
Cryptopp\SymmetricTransformationFilter : constructor was not called
Cryptopp\SymmetricTransformationFilter : constructor was not called
