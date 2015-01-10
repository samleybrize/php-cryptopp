--TEST--
Symmetric cipher interface
--FILE--
<?php

class CipherInterfaceChild implements Cryptopp\SymmetricCipherInterface
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

    public function setKey($key)
    {
        return "key-$key";
    }

    public function getKey()
    {
        return "thekey";
    }
}

$o = new CipherInterfaceChild();
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->isValidKeyLength(3));
var_dump($o->setKey("az"));
var_dump($o->getKey());

?>
--EXPECT--
string(4) "test"
int(5)
bool(false)
string(6) "key-az"
string(6) "thekey"
