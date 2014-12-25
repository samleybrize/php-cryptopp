--TEST--
Hash transformation filter
--FILE--
<?php

$o = new Cryptopp\HashTransformationFilter(new Cryptopp\HashSha1());
var_dump($o->getHash()->getName());

// check final methods
echo "- final methods:\n";
$reflection = new ReflectionClass("Cryptopp\HashTransformationFilter");
$methods    = $reflection->getMethods();

foreach ($methods as $method) {
    if (!$method->isFinal()) {
        continue;
    }

    var_dump($method->getName());
}

// calculate digest
echo "- calculateDigestString:\n";
var_dump(bin2hex($o->calculateDigestString("azertyuiop")));
var_dump(bin2hex($o->calculateDigestString("azertyuiop")));
var_dump(bin2hex($o->calculateDigestString("wxcvbnqsdfg")));

// mac
echo "- mac:\n";
$m = new Cryptopp\MacHmac(new Cryptopp\HashSha1());
$o = new Cryptopp\HashTransformationFilter($m);
$m->setKey("1234567890123456");
var_dump(bin2hex($o->calculateDigestString("azertyuiop")));
var_dump(bin2hex($o->calculateDigestString("azertyuiop")));
var_dump(bin2hex($o->calculateDigestString("wxcvbnqsdfg")));

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
string(4) "sha1"
- final methods:
string(7) "__sleep"
string(8) "__wakeup"
string(7) "getHash"
string(21) "calculateDigestString"
- calculateDigestString:
string(40) "58ad983135fe15c5a8e2e15fb5b501aedcf70dc2"
string(40) "58ad983135fe15c5a8e2e15fb5b501aedcf70dc2"
string(40) "0593a3f434eacdbf5c56faf578b2139ef91e3bc0"
- mac:
string(40) "8254f31ab6a9b0fae9a47205b71396ecc94775e5"
string(40) "8254f31ab6a9b0fae9a47205b71396ecc94775e5"
string(40) "2e9c4a1885c06bffe3c6e745cf688950e021c7e0"
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
