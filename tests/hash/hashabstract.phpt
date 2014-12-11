--TEST--
Hash abstract
--FILE--
<?php

var_dump(is_a("Cryptopp\HashAbstract", "Cryptopp\HashInterface", true));

// check that abstract class cannot be extended
class HashAbstractChild extends Cryptopp\HashAbstract{}
$o = new HashAbstractChild();

try {
    $o->getName();
} catch (Cryptopp\CryptoppException $e) {
    var_dump(get_class($e), $e->getMessage());
}

// check final methods
$reflection = new ReflectionClass("Cryptopp\HashAbstract");
$methods    = $reflection->getMethods(ReflectionMethod::IS_FINAL | ReflectionMethod::IS_PUBLIC);

foreach ($methods as $method) {
    var_dump($method->getName());
}

?>
--EXPECT--
bool(true)
string(26) "Cryptopp\CryptoppException"
string(56) "Cryptopp\HashAbstract cannot be extended by user classes"
string(7) "__sleep"
string(8) "__wakeup"
string(7) "getName"
string(13) "getDigestSize"
string(12) "getBlockSize"
string(15) "calculateDigest"
string(6) "update"
string(8) "finalize"
string(7) "restart"
