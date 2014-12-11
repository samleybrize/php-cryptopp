--TEST--
Mac abstract
--FILE--
<?php

var_dump(is_a("Cryptopp\MacAbstract", "Cryptopp\MacInterface", true));

class MacAbstractChild extends Cryptopp\MacAbstract{}
$o = new MacAbstractChild();

try {
    $o->getName();
} catch (Cryptopp\CryptoppException $e) {
    var_dump(get_class($e), $e->getMessage());
}

?>
--EXPECT--
bool(true)
string(26) "Cryptopp\CryptoppException"
string(55) "Cryptopp\MacAbstract cannot be extended by user classes"
