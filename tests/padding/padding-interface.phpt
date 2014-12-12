--TEST--
Padding interface
--FILE--
<?php

class PaddingInterfaceChild implements Cryptopp\PaddingInterface
{
    public function pad($data, $blockSize)
    {
        return "padded-$blockSize";
    }

    public function unpad($data, $blockSize)
    {
        return "unpadded-$blockSize";
    }

    public function canPad()
    {
        return true;
    }

    public function canUnpad()
    {
        return false;
    }
}

$o = new PaddingInterfaceChild();
var_dump($o->pad("dataaa", 5));
var_dump($o->unpad("dataaa", 7));
var_dump($o->canPad());
var_dump($o->canUnpad());

?>
--EXPECT--
string(8) "padded-5"
string(10) "unpadded-7"
bool(true)
bool(false)
