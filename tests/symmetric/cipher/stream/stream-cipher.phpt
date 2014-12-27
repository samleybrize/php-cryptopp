--TEST--
StreamCipher class
--FILE--
<?php

$algoList = Cryptopp\StreamCipher::getAlgos();
var_dump(gettype($algoList));
var_dump(empty($algoList));

var_dump(Cryptopp\StreamCipher::getClassname("sosemanuk"));

?>
--EXPECT--
string(5) "array"
bool(false)
string(30) "Cryptopp\StreamCipherSosemanuk"
