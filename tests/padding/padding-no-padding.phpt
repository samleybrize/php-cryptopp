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

// bad arguments
echo "- bad arguments:\n";
var_dump(@$padding->pad(array(), 4));
echo "$php_errormsg\n";
var_dump(@$padding->pad("azerty", array()));
echo "$php_errormsg\n";
var_dump(@$padding->unpad(array(), 4));
echo "$php_errormsg\n";
var_dump(@$padding->unpad("azerty", array()));
echo "$php_errormsg\n";

?>
--EXPECT--
bool(true)
string(6) "azerty"
string(6) "qwerty"
string(6) "wxcvbn"
string(6) "qscvbn"
bool(false)
bool(false)
- bad arguments:
bool(false)
Cryptopp\PaddingNoPadding::pad() expects parameter 1 to be string, array given
bool(false)
Cryptopp\PaddingNoPadding::pad() expects parameter 2 to be long, array given
bool(false)
Cryptopp\PaddingNoPadding::unpad() expects parameter 1 to be string, array given
bool(false)
Cryptopp\PaddingNoPadding::unpad() expects parameter 2 to be long, array given
