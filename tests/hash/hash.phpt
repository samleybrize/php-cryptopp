--TEST--
Hash class
--FILE--
<?php

$algoList = Cryptopp\Hash::getAlgos();
var_dump(gettype($algoList));
var_dump(empty($algoList));

var_dump(Cryptopp\Hash::getClassname("md5"));

?>
--EXPECT--
string(5) "array"
bool(false)
string(16) "Cryptopp\HashMd5"
