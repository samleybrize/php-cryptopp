--TEST--
SymmetricMode class
--FILE--
<?php

$algoList = Cryptopp\SymmetricMode::getModes();
var_dump(gettype($algoList));
var_dump(empty($algoList));

var_dump(Cryptopp\SymmetricMode::getClassname("ecb"));

?>
--EXPECT--
string(5) "array"
bool(false)
string(25) "Cryptopp\SymmetricModeEcb"
