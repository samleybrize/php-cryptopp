--TEST--
Hash abstract
--FILE--
<?php

var_dump(is_a("Cryptopp\HashAbstract", "Cryptopp\HashInterface", true));

class HashAbstractChild extends Cryptopp\HashAbstract{}
$o = new HashAbstractChild();

try {
    $o->getName();
} catch (Cryptopp\CryptoppException $e) {
    var_dump(get_class($e), $e->getMessage());
}

?>
--EXPECT--
bool(true)
string(26) "Cryptopp\CryptoppException"
string(56) "Cryptopp\HashAbstract cannot be extended by user classes"
