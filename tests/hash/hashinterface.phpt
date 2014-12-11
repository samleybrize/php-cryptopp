--TEST--
Hash interface
--FILE--
<?php

class HashInterfaceChild implements Cryptopp\HashInterface
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

$o = new HashInterfaceChild();
var_dump($o->getName());
var_dump($o->getDigestSize());
var_dump($o->getBlockSize());
var_dump($o->calculateDigest("a"));
var_dump($o->update("b"));
var_dump($o->finalize());
var_dump($o->restart());

?>
--EXPECT--
bool(true)
string(4) "test"
int(12)
int(5)
string(6) "d!gest"
string(7) "updated"
string(9) "finalized"
string(9) "restarted"
