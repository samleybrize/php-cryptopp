--TEST--
Block cipher interface
--FILE--
<?php

class BlockCipherInterfaceChild implements Cryptopp\BlockCipherInterface
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

    public function setKey($key)
    {
        return "key-$key";
    }

    public function encrypt($data)
    {
        return "encrypted";
    }

    public function decrypt($data)
    {
        return "decrypted";
    }

    public function encryptBlock($block)
    {
        return "block encrypted";
    }

    public function decryptBlock($block)
    {
        return "block decrypted";
    }
}

$o = new BlockCipherInterfaceChild();
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->isValidKeyLength(52));
var_dump($o->setKey("keyyy"));
var_dump($o->encrypt("a"));
var_dump($o->decrypt("b"));
var_dump($o->encryptBlock("a"));
var_dump($o->decryptBlock("b"));

var_dump(is_a("Cryptopp\BlockCipherInterface", "Cryptopp\SymmetricCipherInterface", true));

?>
--EXPECT--
string(4) "test"
int(5)
bool(true)
string(9) "key-keyyy"
string(9) "encrypted"
string(9) "decrypted"
string(15) "block encrypted"
string(15) "block decrypted"
bool(true)
