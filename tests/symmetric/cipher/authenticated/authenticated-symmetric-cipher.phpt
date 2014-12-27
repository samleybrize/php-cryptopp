--TEST--
AuthenticatedSymmetricCipher class
--FILE--
<?php

$algoList = Cryptopp\AuthenticatedSymmetricCipher::getAlgos();
var_dump(gettype($algoList));
var_dump(empty($algoList));

var_dump(Cryptopp\AuthenticatedSymmetricCipher::getClassname("gcm"));

?>
--EXPECT--
string(5) "array"
bool(false)
string(40) "Cryptopp\AuthenticatedSymmetricCipherGcm"
