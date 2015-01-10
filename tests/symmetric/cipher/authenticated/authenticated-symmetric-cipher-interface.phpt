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

    public function addEncryptionAdditionalData($data)
    {
        return "encryption aad";
    }

    public function addDecryptionAdditionalData($data)
    {
        return "decryption aad";
    }

    public function finalizeEncryption()
    {
        return "encryption finalized";
    }

    public function finalizeDecryption()
    {
        return "decryption finalized";
    }

    public function restart()
    {
        return "restarted";
    }
}

$o = new AuthenticatedCipherInterfaceChild();
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->getDigestSize());
var_dump($o->isValidKeyLength(12));
var_dump($o->isValidIvLength(15));
var_dump($o->setKey("keyyy"));
var_dump($o->setIv("ivvv"));
var_dump($o->getKey());
var_dump($o->getIv());
var_dump($o->encrypt("a"));
var_dump($o->decrypt("b"));
var_dump($o->addEncryptionAdditionalData("c"));
var_dump($o->addDecryptionAdditionalData("c"));
var_dump($o->finalizeEncryption());
var_dump($o->finalizeDecryption());
var_dump($o->restart());

var_dump(is_a("Cryptopp\AuthenticatedSymmetricCipherInterface", "Cryptopp\SymmetricTransformationInterface", true));

?>
--EXPECT--
string(4) "test"
int(5)
int(8)
bool(true)
bool(true)
string(9) "key-keyyy"
string(7) "iv-ivvv"
string(6) "thekey"
string(5) "theiv"
string(9) "encrypted"
string(9) "decrypted"
string(14) "encryption aad"
string(14) "decryption aad"
string(20) "encryption finalized"
string(20) "decryption finalized"
string(9) "restarted"
bool(true)
