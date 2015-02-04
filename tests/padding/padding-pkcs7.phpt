--TEST--
Padding: PKCS #7
--FILE--
<?php

var_dump(is_a("Cryptopp\PaddingPkcs7", "Cryptopp\PaddingInterface", true));

$padding = new Cryptopp\PaddingPkcs7();

var_dump(Cryptopp\HexUtils::bin2hex($padding->pad("", 6)));
var_dump(Cryptopp\HexUtils::bin2hex($padding->pad("azerty", 6)));
var_dump(Cryptopp\HexUtils::bin2hex($padding->pad("qwerty", 8)));
var_dump(Cryptopp\HexUtils::bin2hex($padding->pad("wxcvbn", 4)));
var_dump($padding->unpad(Cryptopp\HexUtils::hex2bin("060606060606"), 6));
var_dump($padding->unpad(Cryptopp\HexUtils::hex2bin("617a65727479060606060606"), 6));
var_dump($padding->unpad(Cryptopp\HexUtils::hex2bin("7177657274790202"), 8));
var_dump($padding->unpad(Cryptopp\HexUtils::hex2bin("77786376626e0202"), 8));

$data   = str_repeat("a", 10485760);
$pad    = str_repeat(chr(16), 16);
var_dump(strlen($padding->pad($data, 16)));
var_dump(strlen($padding->unpad($data . $pad, 16)));

var_dump($padding->canPad());
var_dump($padding->canUnpad());

?>
--EXPECT--
bool(true)
string(12) "060606060606"
string(24) "617a65727479060606060606"
string(16) "7177657274790202"
string(16) "77786376626e0202"
string(0) ""
string(6) "azerty"
string(6) "qwerty"
string(6) "wxcvbn"
int(10485776)
int(10485760)
bool(true)
bool(true)
