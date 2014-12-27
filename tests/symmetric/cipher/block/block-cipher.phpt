--TEST--
BlockCipher class
--FILE--
<?php

$algoList = Cryptopp\BlockCipher::getAlgos();
var_dump(gettype($algoList));
var_dump(empty($algoList));

var_dump(Cryptopp\BlockCipher::getClassname("aes"));

?>
--EXPECT--
string(5) "array"
bool(false)
string(23) "Cryptopp\BlockCipherAes"
