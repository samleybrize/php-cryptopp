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
echo "\n";
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

// test inheritance
echo "\n";
class Child extends Cryptopp\RandomByteGenerator
{
    public function __construct(){}
}

$o = new Child();

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

Size must be a positive integer, 0 given
Size must be a positive integer, -1 given

Parent constructor was not called
