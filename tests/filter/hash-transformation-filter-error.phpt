--TEST--
Hash transformation filter (errors)
--FILE--
<?php

$m = new Cryptopp\MacHmac(new Cryptopp\HashSha1());
$o = new Cryptopp\HashTransformationFilter($m);
$m->setKey("1234567890123456");

// sleep
echo "- sleep:\n";
try {
    serialize($o);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// bad arguments
echo "- bad arguments:\n";
var_dump(@$o->calculateDigestString(array()));
echo "$php_errormsg\n";

// hash object: constructor not called
echo "- hash object (parent constructor not called):\n";
class HashChild extends Cryptopp\HashSha1
{
    public function __construct(){}
}

try {
    $o = new Cryptopp\HashTransformationFilter(new HashChild());
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// mac object: constructor not called
echo "- mac object (parent constructor not called):\n";
class MacChild extends Cryptopp\MacHmac
{
    public function __construct(){}
}

try {
    $o = new Cryptopp\HashTransformationFilter(new MacChild());
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\HashTransformationFilter
{
    public function __construct(){}
}

$o = new ChildParentConstructorNotCalled();

try {
    $o->calculateDigestString("");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
- sleep:
You cannot serialize or unserialize Cryptopp\HashTransformationFilter instances
- bad arguments:
NULL
Cryptopp\HashTransformationFilter::calculateDigestString() expects parameter 1 to be string, array given
- hash object (parent constructor not called):
HashChild : parent constructor was not called
- mac object (parent constructor not called):
MacChild : parent constructor was not called
- inheritance (parent constructor not called):
Cryptopp\HashTransformationFilter : constructor was not called
