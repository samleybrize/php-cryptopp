--TEST--
Symmetric mode interface
--FILE--
<?php

class ModeInterfaceChild implements Cryptopp\SymmetricModeInterface
{
    public function getName()
    {
        return "test";
    }

    public function getBlockSize()
    {
        return 5;
    }

    public function isValidKeyLength($keyLength)
    {
        return true;
    }

    public function isValidIvLength($length)
    {
        return true;
    }

    public function setKey($key)
    {
        return "key-$key";
    }

    public function setIv($iv)
    {
        return "iv-$iv";
    }

    public function encrypt($data)
    {
        return "encrypted";
    }

    public function decrypt($data)
    {
        return "decrypted";
    }

    public function restart()
    {
        return "restarted";
    }
}

$o = new ModeInterfaceChild();
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->isValidKeyLength(12));
var_dump($o->isValidIvLength(15));
var_dump($o->setKey("keyyy"));
var_dump($o->setIv("ivvv"));
var_dump($o->encrypt("a"));
var_dump($o->decrypt("b"));
var_dump($o->restart());

var_dump(is_a("Cryptopp\SymmetricModeInterface", "Cryptopp\SymmetricTransformationInterface", true));

?>
--EXPECT--
string(4) "test"
int(5)
bool(true)
bool(true)
string(9) "key-keyyy"
string(7) "iv-ivvv"
string(9) "encrypted"
string(9) "decrypted"
string(9) "restarted"
bool(true)
