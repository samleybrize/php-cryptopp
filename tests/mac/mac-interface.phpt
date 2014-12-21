--TEST--
Mac interface
--FILE--
<?php

class MacInterfaceChild implements Cryptopp\MacInterface
{
    public function getName()
    {
        return "test";
    }

    public function getDigestSize()
    {
        return 12;
    }

    public function getBlockSize()
    {
        return 5;
    }

    public function setKey($key)
    {
        return $key;
    }

    public function calculateDigest($data)
    {
        return "d!gest";
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
}

$o = new MacInterfaceChild();
var_dump($o->getName());
var_dump($o->getDigestSize());
var_dump($o->getBlockSize());
var_dump($o->setKey("keyyy"));
var_dump($o->calculateDigest("a"));
var_dump($o->update("b"));
var_dump($o->finalize());
var_dump($o->restart());

var_dump(is_a("Cryptopp\MacInterface", "Cryptopp\HashTransformationInterface", true));

?>
--EXPECT--
string(4) "test"
int(12)
int(5)
string(5) "keyyy"
string(6) "d!gest"
string(7) "updated"
string(9) "finalized"
string(9) "restarted"
bool(true)
