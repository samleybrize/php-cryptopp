--TEST--
Symmetric transformation filter with authenticated cipher
--FILE--
<?php

$a = new Cryptopp\AuthenticatedSymmetricCipherGcm(new Cryptopp\BlockCipherAes());
$o = new Cryptopp\SymmetricTransformationFilter($a);
var_dump($o->getCipher()->getName());

// encryptString
echo "- encryptString:\n";
$a->setKey("1234567890123456");
$a->setIv("1234567890123456");
var_dump(bin2hex($o->encryptString("azertyuiop")));
var_dump(bin2hex($o->encryptString("azertyuiop")));
var_dump(bin2hex($o->encryptString("wxcvbnqsdfg")));

// decryptString
echo "- decryptString:\n";
var_dump($o->decryptString(hex2bin("e7de80cef314ceaefb14")));
var_dump($o->decryptString(hex2bin("e7de80cef314ceaefb14")));
var_dump($o->decryptString(hex2bin("f1dc86cae503cab4f002de")));

?>
--EXPECT--
string(8) "gcm(aes)"
- encryptString:
string(20) "e7de80cef314ceaefb14"
string(20) "e7de80cef314ceaefb14"
string(22) "f1dc86cae503cab4f002de"
- decryptString:
string(10) "azertyuiop"
string(10) "azertyuiop"
string(11) "wxcvbnqsdfg"
