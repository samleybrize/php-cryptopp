--TEST--
Stream cipher abstract
--FILE--
<?php

var_dump(is_a("Cryptopp\StreamCipherAbstract", "Cryptopp\StreamCipherInterface", true));

class StreamCipherAbstractChild extends Cryptopp\StreamCipherAbstract{}
$o = new StreamCipherAbstractChild();

try {
    $o->getName();
} catch (Cryptopp\CryptoppException $e) {
    var_dump(get_class($e), $e->getMessage());
}

?>
--EXPECT--
bool(true)
string(26) "Cryptopp\CryptoppException"
string(64) "Cryptopp\StreamCipherAbstract cannot be extended by user classes"
