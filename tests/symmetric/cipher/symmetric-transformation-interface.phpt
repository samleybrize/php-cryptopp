--TEST--
Symmetric transformation interface
--FILE--
<?php

class TransformationInterfaceChild implements Cryptopp\SymmetricTransformationInterface
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
        return false;
    }

    public function isValidIvLength($keyLength)
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

    public function getKey()
    {
        return "thekey";
    }

    public function getIv()
    {
        return "theiv";
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

$o = new TransformationInterfaceChild();
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->isValidKeyLength(13));
var_dump($o->isValidIvLength(17));
var_dump($o->setKey("keyyy"));
var_dump($o->setIv("ivvv"));
var_dump($o->getKey());
var_dump($o->getIv());
var_dump($o->encrypt("a"));
var_dump($o->decrypt("b"));
var_dump($o->restart());

var_dump(is_a("Cryptopp\SymmetricTransformationInterface", "Cryptopp\SymmetricCipherInterface", true));

?>
--EXPECT--
string(4) "test"
int(5)
bool(false)
bool(true)
string(9) "key-keyyy"
string(7) "iv-ivvv"
string(6) "thekey"
string(5) "theiv"
string(9) "encrypted"
string(9) "decrypted"
string(9) "restarted"
bool(true)
