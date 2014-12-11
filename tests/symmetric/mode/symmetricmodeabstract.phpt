--TEST--
Symmetric mode abstract
--FILE--
<?php

var_dump(is_a("Cryptopp\SymmetricModeAbstract", "Cryptopp\SymmetricModeInterface", true));

class ModeAbstractChild extends Cryptopp\SymmetricModeAbstract{}
$o = new ModeAbstractChild();

try {
    $o->getName();
} catch (Cryptopp\CryptoppException $e) {
    var_dump(get_class($e), $e->getMessage());
}

?>
--EXPECT--
bool(true)
string(26) "Cryptopp\CryptoppException"
string(65) "Cryptopp\SymmetricModeAbstract cannot be extended by user classes"
