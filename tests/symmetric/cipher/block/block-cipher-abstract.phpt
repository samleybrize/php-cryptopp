--TEST--
Block cipher abstract
--FILE--
<?php

var_dump(is_a("Cryptopp\BlockCipherAbstract", "Cryptopp\BlockCipherInterface", true));

// check that abstract class cannot be extended
class BlockCipherAbstractChild extends Cryptopp\BlockCipherAbstract{}
$o = new BlockCipherAbstractChild();

try {
    $o->getName();
} catch (Cryptopp\CryptoppException $e) {
    var_dump($e->getMessage());
}

// check final methods
echo "- final methods:\n";
$reflection = new ReflectionClass("Cryptopp\BlockCipherAbstract");
$methods    = $reflection->getMethods();

foreach ($methods as $method) {
    if (!$method->isFinal()) {
        continue;
    }

    var_dump($method->getName());
}

?>
--EXPECT--
bool(true)
string(63) "Cryptopp\BlockCipherAbstract cannot be extended by user classes"
- final methods:
string(7) "__sleep"
string(8) "__wakeup"
string(7) "getName"
string(12) "getBlockSize"
string(16) "isValidKeyLength"
string(6) "setKey"
string(6) "getKey"
string(12) "encryptBlock"
string(12) "decryptBlock"
string(7) "encrypt"
string(7) "decrypt"
