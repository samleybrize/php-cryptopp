--TEST--
Mac abstract
--FILE--
<?php

var_dump(is_a("Cryptopp\MacAbstract", "Cryptopp\MacInterface", true));

// check that abstract class cannot be extended
class MacAbstractChild extends Cryptopp\MacAbstract{}
$o = new MacAbstractChild();

try {
    $o->getName();
} catch (Cryptopp\CryptoppException $e) {
    var_dump(get_class($e), $e->getMessage());
}

// check final methods
$reflection = new ReflectionClass("Cryptopp\MacAbstract");
$methods    = $reflection->getMethods(ReflectionMethod::IS_FINAL | ReflectionMethod::IS_PUBLIC);

foreach ($methods as $method) {
    var_dump($method->getName());
}

?>
--EXPECT--
bool(true)
string(26) "Cryptopp\CryptoppException"
string(55) "Cryptopp\MacAbstract cannot be extended by user classes"
string(7) "__sleep"
string(8) "__wakeup"
string(7) "getName"
string(13) "getDigestSize"
string(6) "setKey"
string(15) "calculateDigest"
string(6) "update"
string(8) "finalize"
string(7) "restart"
string(6) "verify"
