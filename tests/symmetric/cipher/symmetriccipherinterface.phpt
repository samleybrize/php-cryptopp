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
}

$o = new CipherInterfaceChild();
var_dump($o->getName());

?>
--EXPECT--
string(4) "test"
