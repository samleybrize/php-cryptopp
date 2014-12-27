--TEST--
Extension MInfo
--FILE--
<?php

$r = new ReflectionExtension("crypto++");
$r->info();

?>
--EXPECTF--
crypto++

Crypto++ support => enabled
Crypto++ module version => %s
Crypto++ library version => %s
Supported hash algorithms => %s
Supported MAC algorithms => %s
Supported stream ciphers => %s
Supported block ciphers => %s
Supported cipher modes => %s
Supported authenticated encryption schemes => %s
