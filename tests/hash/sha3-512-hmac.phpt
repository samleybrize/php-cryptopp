--TEST--
Hash algorithm: HMAC-SHA3-512
--FILE--
<?php

$hash = new Cryptopp\HashSha3_512();
$hmac = new Cryptopp\MacHmac($hash);

$hmac->setKey(hex2bin("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b"));
var_dump(bin2hex($hmac->calculateDigest("Hi There")));

$hmac->setKey("Jefe");
var_dump(bin2hex($hmac->calculateDigest("what do ya want for nothing?")));

$hmac->setKey(hex2bin("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
var_dump(bin2hex($hmac->calculateDigest(hex2bin(str_repeat("dd", 50)))));

$hmac->setKey(hex2bin("0102030405060708090a0b0c0d0e0f10111213141516171819"));
var_dump(bin2hex($hmac->calculateDigest(hex2bin(str_repeat("cd", 50)))));

$hmac->setKey(hex2bin("0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c"));
var_dump(bin2hex($hmac->calculateDigest("Test With Truncation")));

$hmac->setKey(hex2bin(str_repeat("aa", 160)));
var_dump(bin2hex($hmac->calculateDigest("Test Using Larger Than Block-Size Key - Hash Key First")));

$hmac->setKey(hex2bin(str_repeat("aa", 160)));
var_dump(bin2hex($hmac->calculateDigest("Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data 1234567890123456789012345678901234567890123456789012345678901234567890123456")));

?>
--EXPECTF--
string(128) "cc1871ebcff2546bdc0caf831c60a2d4b56168ace2d87a0b2076ac97980d0a098a6c9b09c257e53d6baabc73c76827ce16476e8e8b25bcdcebddeba3e956c8a1"
string(128) "6fc9491b596d434481fae1d3bac8a91a2cac4a431965e91f9fe8b48138f5d11e554347cb5253bab5712239abe3e0945069487dcdc22b040e8fae90e2b10cbfe0"
string(128) "fa1f21595067d7990bc8f3480e782bbc0664269a1ace2497746503c1c357697ae079b980980223324d8323892d1c5011a69a9d5e243c7a8c00eb887c1fb156a7"
string(128) "468521f82e2682555a20fe0680020f4e4a2893d64ac69f2ef3bee2d81fb67128026ca175b0a3d12a4ba2bb104e93bd1f06b2bdd308c23fcb2bc4bd0a83cbb92a"
string(128) "649b5a05c2a9eac8db57906a66517cdf3ecd49a132f15a81075a6340da9d6f4cdd8a1d223c45d6d4aace1bf364d28130eb94db1a701092a002e30938181a9127"
string(128) "a520864794408b6e1397adde9d7c12cf98748afc4a471bf7bf9526d2f36170c1efc359a8392f24275c1384b7915d911e85c5fed833c588557b43a3f82df188b5"
string(128) "a25921858fe691f15cdefbf5b899db4578597d0de4c01bbb4784b37e7da7d5f58858f6c69a3a3d7319ec1f4a8c8b75a7bcae5025474ce78094d95be1dd5106b8"
