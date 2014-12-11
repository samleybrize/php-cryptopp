--TEST--
Block cipher abstract
--FILE--
<?php

var_dump(is_a("Cryptopp\BlockCipherAbstract", "Cryptopp\BlockCipherInterface", true));

class BlockCipherAbstractChild extends Cryptopp\BlockCipherAbstract{}
$o = new BlockCipherAbstractChild();

try {
    $o->getName();
} catch (Cryptopp\CryptoppException $e) {
    var_dump(get_class($e), $e->getMessage());
}

?>
--EXPECT--
bool(true)
string(26) "Cryptopp\CryptoppException"
string(63) "Cryptopp\BlockCipherAbstract cannot be extended by user classes"
