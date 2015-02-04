--TEST--
Stream cipher: Salsa20
--FILE--
<?php

var_dump(is_a("Cryptopp\StreamCipherSalsa20", "Cryptopp\StreamCipherAbstract", true));

// check algorithm infos
$o = new Cryptopp\StreamCipherSalsa20();
var_dump($o->getName());
var_dump($o->getBlockSize());

// key length check
echo "- key length check:\n";
var_dump($o->isValidKeyLength(16));
var_dump($o->isValidKeyLength(32));
var_dump($o->isValidKeyLength(9));
var_dump($o->isValidKeyLength(0));
var_dump($o->isValidKeyLength(33));

// iv length check
echo "- iv length check:\n";
var_dump($o->isValidIvLength(8));
var_dump($o->isValidIvLength(16));
var_dump($o->isValidIvLength(17));

// set key
echo "- set key:\n";
$o->setKey("azertyuiopqsdfgh");
var_dump($o->getKey());

// set iv
echo "- set iv:\n";
$o->setIv("qsdfghjk");
var_dump($o->getIv());

// encrypt
echo "- encrypt:\n";
$o->setKey(Cryptopp\HexUtils::hex2bin("a7c083feb7aabbff1122334455667788"));
$o->setIv(Cryptopp\HexUtils::hex2bin("0011223344556677"));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));

// decrypt
echo "- decrypt:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("03ad21c7d699f686d0039c12603242bf"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("a566b2365dfb197b61a549b299e2149a"))));

// encrypt (12 rounds)
echo "- encrypt 12 rounds:\n";
$o->setRounds(12);
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));

// decrypt (12 rounds)
echo "- decrypt 12 rounds:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("c7619d385ff57fad9c9ab9e7d2fa6334"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("9067a70e21e095f254c482f8e0fc2161"))));

// encrypt (8 rounds)
echo "- encrypt 8 rounds:\n";
$o->setRounds(8);
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));

// decrypt (8 rounds)
echo "- decrypt 8 rounds:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("4e26530489778baff97e6c8c6f650e3f"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("f9e95b4282bac9253d552bad7f7890a6"))));

// restart encryption
echo "- restart encryption:\n";
$o->restart();
$o->setRounds(20);
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));

// restart decryption
echo "- restart decryption:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("03ad21c7d699f686d0039c12603242bf"))));
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("a566b2365dfb197b61a549b299e2149a"))));

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485760);
var_dump(strlen($o->encrypt($data)));
var_dump(strlen($o->decrypt($data)));

?>
--EXPECT--
bool(true)
string(7) "salsa20"
int(1)
- key length check:
bool(true)
bool(true)
bool(false)
bool(false)
bool(false)
- iv length check:
bool(true)
bool(false)
bool(false)
- set key:
string(16) "azertyuiopqsdfgh"
- set iv:
string(8) "qsdfghjk"
- encrypt:
string(32) "03ad21c7d699f686d0039c12603242bf"
string(32) "a566b2365dfb197b61a549b299e2149a"
- decrypt:
string(32) "00000000000000000000000000000000"
string(32) "00000000000000000000000000000000"
- encrypt 12 rounds:
string(32) "c7619d385ff57fad9c9ab9e7d2fa6334"
string(32) "9067a70e21e095f254c482f8e0fc2161"
- decrypt 12 rounds:
string(32) "00000000000000000000000000000000"
string(32) "00000000000000000000000000000000"
- encrypt 8 rounds:
string(32) "4e26530489778baff97e6c8c6f650e3f"
string(32) "f9e95b4282bac9253d552bad7f7890a6"
- decrypt 8 rounds:
string(32) "00000000000000000000000000000000"
string(32) "00000000000000000000000000000000"
- restart encryption:
string(32) "03ad21c7d699f686d0039c12603242bf"
string(32) "03ad21c7d699f686d0039c12603242bf"
- restart decryption:
string(32) "00000000000000000000000000000000"
string(32) "a6cb93f18b62effdb1a6d5a0f9d05625"
- large data:
int(10485760)
int(10485760)
