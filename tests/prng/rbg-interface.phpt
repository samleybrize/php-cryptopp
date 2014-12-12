--TEST--
Random byte generator interface
--FILE--
<?php

class RbgInterfaceChild implements Cryptopp\RandomByteGeneratorInterface
{
    public function generate($size)
    {
        return "random-$size";
    }
}

$o = new RbgInterfaceChild();
var_dump($o->generate(5));

?>
--EXPECT--
string(8) "random-5"
