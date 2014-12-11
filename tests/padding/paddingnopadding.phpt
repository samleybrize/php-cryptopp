--TEST--
Padding: no padding
--FILE--
<?php

var_dump(is_a("Cryptopp\PaddingNoPadding", "Cryptopp\PaddingInterface", true));

$padding = new Cryptopp\PaddingNoPadding();

var_dump($padding->pad("azerty", 6));
var_dump($padding->pad("qwerty", 8));
var_dump($padding->unpad("wxcvbn", 6));
var_dump($padding->unpad("qscvbn", 8));

var_dump($padding->canPad());
var_dump($padding->canUnpad());

?>
--EXPECT--
bool(true)
string(6) "azerty"
string(6) "qwerty"
string(6) "wxcvbn"
string(6) "qscvbn"
bool(false)
bool(false)
