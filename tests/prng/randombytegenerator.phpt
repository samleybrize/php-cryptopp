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

?>
--EXPECT--
bool(true)
int(5)
int(5)
int(8)
bool(true)
