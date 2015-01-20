--TEST--
Authenticated symmetric transformation filter (errors)
--FILE--
<?php

$c = new Cryptopp\BlockCipherAes();
$a = new Cryptopp\AuthenticatedSymmetricCipherGcm($c);
$o = new Cryptopp\AuthenticatedSymmetricTransformationFilter($a);
$a->setKey(hex2bin("77be63708971c4e240d1cb79e8d77feb"));
$a->setIv(hex2bin("e0e00f19fed7ba0136a797f3"));

// mac verification failed
echo "- mac verification failed:\n";
try {
    $o->decryptString("12345678901234567890123456789012", hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"));
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
var_dump(@$o->encryptString(array()));
echo "$php_errormsg\n";
var_dump(@$o->encryptString("a", array()));
echo "$php_errormsg\n";
var_dump(@$o->decryptString(array()));
echo "$php_errormsg\n";
var_dump(@$o->decryptString("e", array()));
echo "$php_errormsg\n";

// cipher object: constructor not called
echo "- cipher object (parent constructor not called):\n";
class AuthenticatedChild extends Cryptopp\AuthenticatedSymmetricCipherGcm
{
    public function __construct(){}
}

try {
    $o = new Cryptopp\AuthenticatedSymmetricTransformationFilter(new AuthenticatedChild());
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\AuthenticatedSymmetricTransformationFilter
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
- mac verification failed:
Cryptopp\AuthenticatedSymmetricTransformationFilter : MAC verification failed
- sleep:
You cannot serialize or unserialize Cryptopp\AuthenticatedSymmetricTransformationFilter instances
- bad arguments:
NULL
Cryptopp\AuthenticatedSymmetricTransformationFilter::encryptString() expects parameter 1 to be string, array given
NULL
Cryptopp\AuthenticatedSymmetricTransformationFilter::encryptString() expects parameter 2 to be string, array given
NULL
Cryptopp\AuthenticatedSymmetricTransformationFilter::decryptString() expects parameter 1 to be string, array given
NULL
Cryptopp\AuthenticatedSymmetricTransformationFilter::decryptString() expects parameter 2 to be string, array given
- cipher object (parent constructor not called):
AuthenticatedChild : parent constructor was not called
- inheritance (parent constructor not called):
Cryptopp\AuthenticatedSymmetricTransformationFilter : constructor was not called
Cryptopp\AuthenticatedSymmetricTransformationFilter : constructor was not called
