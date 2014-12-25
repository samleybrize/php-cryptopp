--TEST--
Random byte generator
--FILE--
<?php

var_dump(is_a("Cryptopp\RandomByteGenerator", "Cryptopp\RandomByteGeneratorInterface", true));

$rbg    = new Cryptopp\RandomByteGenerator();
$gen1   = $rbg->generate(5);
$gen2   = $rbg->generate(5);
$gen3   = $rbg->generate(8);

var_dump(strlen($gen1));
var_dump(strlen($gen2));
var_dump(strlen($gen3));
var_dump($gen1 != $gen2);

// test errors
echo "- errors:\n";
try {
    $rbg->generate(0);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $rbg->generate(-1);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// bad arguments
echo "- bad arguments:\n";
var_dump(@$rbg->generate(array()));
echo "$php_errormsg\n";

// test inheritance
echo "- inheritance:\n";
class Child extends Cryptopp\RandomByteGenerator{}

$o = new Child();
var_dump(strlen($o->generate(2)));

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\RandomByteGenerator
{
    public function __construct(){}
}

$o = new ChildParentConstructorNotCalled();

try {
    $o->generate(2);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
bool(true)
int(5)
int(5)
int(8)
bool(true)
- errors:
Size must be a positive integer, 0 given
Size must be a positive integer, -1 given
- bad arguments:
NULL
Cryptopp\RandomByteGenerator::generate() expects parameter 1 to be long, array given
- inheritance:
int(2)
- inheritance (parent constructor not called):
Parent constructor was not called
