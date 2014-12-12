--TEST--
Stream cipher abstract
--FILE--
<?php

var_dump(is_a("Cryptopp\StreamCipherAbstract", "Cryptopp\StreamCipherInterface", true));

// check that abstract class cannot be extended
class StreamCipherAbstractChild extends Cryptopp\StreamCipherAbstract{}
$o = new StreamCipherAbstractChild();

try {
    $o->getName();
} catch (Cryptopp\CryptoppException $e) {
    var_dump($e->getMessage());
}

// check final methods
echo "- final methods:\n";
$reflection = new ReflectionClass("Cryptopp\StreamCipherAbstract");
$methods    = $reflection->getMethods(ReflectionMethod::IS_FINAL | ReflectionMethod::IS_PUBLIC);

foreach ($methods as $method) {
    var_dump($method->getName());
}

?>
--EXPECT--
bool(true)
string(64) "Cryptopp\StreamCipherAbstract cannot be extended by user classes"
- final methods:
string(7) "__sleep"
string(8) "__wakeup"
string(7) "getName"
string(12) "getBlockSize"
string(16) "isValidKeyLength"
string(6) "setKey"
string(5) "setIv"
string(7) "encrypt"
string(7) "decrypt"
string(7) "restart"
