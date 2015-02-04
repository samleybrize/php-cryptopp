--TEST--
HexUtils
--FILE--
<?php

// bin2hex
echo "- bin2hex:\n";
var_dump(Cryptopp\HexUtils::bin2hex("\0\5\8\9azerty"));

// hex2bin
echo "- hex2bin:\n";
var_dump(Cryptopp\HexUtils::hex2bin("617a6572747975696f70"));

?>
--EXPECT--
- bin2hex:
string(24) "00055c385c39617a65727479"
- hex2bin:
string(10) "azertyuiop"
