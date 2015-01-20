--TEST--
Symmetric transformation filter
--FILE--
<?php

$o = new Cryptopp\SymmetricTransformationFilter(new Cryptopp\StreamCipherSosemanuk());
var_dump($o->getCipher()->getName());

// check default padding
var_dump(get_class($o->getPadding()));

// check final methods
echo "- final methods:\n";
$reflection = new ReflectionClass("Cryptopp\SymmetricTransformationFilter");
$methods    = $reflection->getMethods();

foreach ($methods as $method) {
    if (!$method->isFinal()) {
        continue;
    }

    var_dump($method->getName());
}

// encryptString
echo "- encryptString:\n";
$m = new Cryptopp\SymmetricModeEcb(new Cryptopp\BlockCipherAes());
$o = new Cryptopp\SymmetricTransformationFilter($m, new Cryptopp\PaddingPkcs7());
$m->setKey("1234567890123456");
var_dump(bin2hex($o->encryptString("azertyuiop")));
var_dump(bin2hex($o->encryptString("azertyuiop")));
var_dump(bin2hex($o->encryptString("wxcvbnqsdfg")));

// decryptString
echo "- decryptString:\n";
var_dump($o->decryptString(hex2bin("ce86616e5e9d59edfc4ee2cc002f601b")));
var_dump($o->decryptString(hex2bin("ce86616e5e9d59edfc4ee2cc002f601b")));
var_dump($o->decryptString(hex2bin("da7885b5ad196250c037599a7bf9d62a")));

// stream cipher
echo "- stream cipher:\n";
$c = new Cryptopp\StreamCipherSosemanuk();
$o = new Cryptopp\SymmetricTransformationFilter($c, new Cryptopp\PaddingPkcs7());
$c->setKey("1234567890123456");
$c->setIv("0987654321987654");
var_dump(bin2hex($o->encryptString("azertyuiop")));
var_dump(bin2hex($o->encryptString("azertyuiop")));
var_dump($o->decryptString(hex2bin("76312c85afc0bbdf7821")));
var_dump($o->decryptString(hex2bin("76312c85afc0bbdf7821")));

?>
--EXPECT--
string(9) "sosemanuk"
string(21) "Cryptopp\PaddingPkcs7"
- final methods:
string(7) "__sleep"
string(8) "__wakeup"
string(9) "getCipher"
string(10) "getPadding"
string(13) "encryptString"
string(13) "decryptString"
- encryptString:
string(32) "ce86616e5e9d59edfc4ee2cc002f601b"
string(32) "ce86616e5e9d59edfc4ee2cc002f601b"
string(32) "da7885b5ad196250c037599a7bf9d62a"
- decryptString:
string(10) "azertyuiop"
string(10) "azertyuiop"
string(11) "wxcvbnqsdfg"
- stream cipher:
string(20) "76312c85afc0bbdf7821"
string(20) "76312c85afc0bbdf7821"
string(10) "azertyuiop"
string(10) "azertyuiop"
