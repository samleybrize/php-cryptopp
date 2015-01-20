--TEST--
Symmetric transformation filter (errors)
--FILE--
<?php

$c = new Cryptopp\StreamCipherSosemanuk();
$o = new Cryptopp\SymmetricTransformationFilter($c, new Cryptopp\PaddingPkcs7());
$c->setKey("1234567890123456");
$c->setIv("0987654321987654");

// sleep
echo "- sleep:\n";
try {
    serialize($o);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// bad arguments
echo "- bad arguments:\n";
var_dump(@$o->encryptString(array()));
echo "$php_errormsg\n";
var_dump(@$o->decryptString(array()));
echo "$php_errormsg\n";

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
- sleep:
You cannot serialize or unserialize Cryptopp\SymmetricTransformationFilter instances
- bad arguments:
NULL
Cryptopp\SymmetricTransformationFilter::encryptString() expects parameter 1 to be string, array given
NULL
Cryptopp\SymmetricTransformationFilter::decryptString() expects parameter 1 to be string, array given
- mode object (parent constructor not called):
ModeChild : parent constructor was not called
- stream cipher object (parent constructor not called):
StreamChild : parent constructor was not called
- inheritance (parent constructor not called):
Cryptopp\SymmetricTransformationFilter : constructor was not called
Cryptopp\SymmetricTransformationFilter : constructor was not called
