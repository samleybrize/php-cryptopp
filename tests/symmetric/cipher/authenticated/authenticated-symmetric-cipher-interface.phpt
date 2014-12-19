--TEST--
Authenticated symmetric cipher interface
--FILE--
<?php

class AuthenticatedCipherInterfaceChild implements Cryptopp\AuthenticatedSymmetricCipherInterface
{
    public function getName()
    {
        return "test";
    }

    public function getBlockSize()
    {
        return 5;
    }

    public function getDigestSize()
    {
        return 8;
    }

    public function isValidKeyLength($keyLength)
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

    public function calculateDigest($data)
    {
        return "digest";
    }

    public function update($data)
    {
        return "updated";
    }

    public function finalize()
    {
        return "finalized";
    }

    public function restart()
    {
        return "restarted";
    }

    public function verify($a, $b)
    {
        // TODO todel
    }
}

$o = new AuthenticatedCipherInterfaceChild();
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->getDigestSize());
var_dump($o->isValidKeyLength(12));
var_dump($o->setKey("keyyy"));
var_dump($o->setIv("ivvv"));
var_dump($o->encrypt("a"));
var_dump($o->decrypt("b"));
var_dump($o->calculateDigest("azerty"));
var_dump($o->update("c"));
var_dump($o->finalize());
var_dump($o->restart());

var_dump(is_a("Cryptopp\AuthenticatedSymmetricCipherInterface", "Cryptopp\SymmetricTransformationInterface", true));
var_dump(is_a("Cryptopp\AuthenticatedSymmetricCipherInterface", "Cryptopp\MacInterface", true));

?>
--EXPECT--
string(4) "test"
int(5)
int(8)
bool(true)
string(9) "key-keyyy"
string(7) "iv-ivvv"
string(9) "encrypted"
string(9) "decrypted"
string(6) "digest"
string(7) "updated"
string(9) "finalized"
string(9) "restarted"
bool(true)
bool(true)
