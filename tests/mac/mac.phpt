--TEST--
Mac class
--FILE--
<?php

$algoList = Cryptopp\Mac::getAlgos();
var_dump(gettype($algoList));
var_dump(empty($algoList));

var_dump(Cryptopp\Mac::getClassname("hmac"));

?>
--EXPECT--
string(5) "array"
bool(false)
string(16) "Cryptopp\MacHmac"
