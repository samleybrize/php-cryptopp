--TEST--
Authenticated symmetric cipher abstract
--FILE--
<?php

var_dump(is_a("Cryptopp\AuthenticatedSymmetricCipherAbstract", "Cryptopp\AuthenticatedSymmetricCipherInterface", true));

// check that abstract class cannot be extended
class AuthenticatedCipherAbstractChild extends Cryptopp\AuthenticatedSymmetricCipherAbstract{}
$o = new AuthenticatedCipherAbstractChild();

try {
    $o->getName();
} catch (Cryptopp\CryptoppException $e) {
    var_dump($e->getMessage());
}

// check final methods
echo "- final methods:\n";
$reflection = new ReflectionClass("Cryptopp\AuthenticatedSymmetricCipherAbstract");
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
string(80) "Cryptopp\AuthenticatedSymmetricCipherAbstract cannot be extended by user classes"
- final methods:
string(7) "__sleep"
string(8) "__wakeup"
string(7) "getName"
string(12) "getBlockSize"
string(13) "getDigestSize"
string(16) "isValidKeyLength"
string(6) "setKey"
string(5) "setIv"
string(7) "encrypt"
string(7) "decrypt"
string(15) "calculateDigest"
string(6) "update"
string(8) "finalize"
string(7) "restart"
string(6) "verify"
