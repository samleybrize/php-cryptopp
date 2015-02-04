--TEST--
Stream cipher: XSalsa20
--FILE--
<?php

var_dump(is_a("Cryptopp\StreamCipherXSalsa20", "Cryptopp\StreamCipherAbstract", true));

// check algorithm infos
$o = new Cryptopp\StreamCipherXSalsa20();
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
var_dump($o->isValidIvLength(24));
var_dump($o->isValidIvLength(16));
var_dump($o->isValidIvLength(26));

// set key
echo "- set key:\n";
$o->setKey("azertyuiopqsdfghjklmwxcvbnwqaxsz");
var_dump($o->getKey());

// set iv
echo "- set iv:\n";
$o->setIv("qsdfghjklmwxcvbnazertyui");
var_dump($o->getIv());

// encrypt
echo "- encrypt:\n";
$o->setKey(Cryptopp\HexUtils::hex2bin("2ecbb5a282ee515b3226952d11d0579607f653a708d18920d18dc5106f76074f"));
$o->setIv(Cryptopp\HexUtils::hex2bin("53f67a3bada58382426b7d2142c327c7a9fa75a8634463c7"));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));

// decrypt
echo "- decrypt:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("f097c0d167658edf4fbb3a9dd5ad2b71"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("39b420e15e86515fa935e6b32cf727c4"))));

// encrypt (12 rounds)
echo "- encrypt 12 rounds:\n";
$o->setRounds(12);
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));

// decrypt (12 rounds)
echo "- decrypt 12 rounds:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("4ceb42a120607593198ef1211eec957d"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("1a66979d67c996203e881a0b2e2120a7"))));

// encrypt (8 rounds)
echo "- encrypt 8 rounds:\n";
$o->setRounds(8);
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));

// decrypt (8 rounds)
echo "- decrypt 8 rounds:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("50b85a918f837766062ad25b5a3bc542"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("4fcbd495ecfb35d22e2e7b3e301baf7f"))));

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
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("f097c0d167658edf4fbb3a9dd5ad2b71"))));
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("39b420e15e86515fa935e6b32cf727c4"))));

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485760);
var_dump(strlen($o->encrypt($data)));
var_dump(strlen($o->decrypt($data)));

?>
--EXPECT--
bool(true)
string(8) "xsalsa20"
int(1)
- key length check:
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
- iv length check:
bool(true)
bool(false)
bool(false)
- set key:
string(32) "azertyuiopqsdfghjklmwxcvbnwqaxsz"
- set iv:
string(24) "qsdfghjklmwxcvbnazertyui"
- encrypt:
string(32) "f097c0d167658edf4fbb3a9dd5ad2b71"
string(32) "39b420e15e86515fa935e6b32cf727c4"
- decrypt:
string(32) "00000000000000000000000000000000"
string(32) "00000000000000000000000000000000"
- encrypt 12 rounds:
string(32) "4ceb42a120607593198ef1211eec957d"
string(32) "1a66979d67c996203e881a0b2e2120a7"
- decrypt 12 rounds:
string(32) "00000000000000000000000000000000"
string(32) "00000000000000000000000000000000"
- encrypt 8 rounds:
string(32) "50b85a918f837766062ad25b5a3bc542"
string(32) "4fcbd495ecfb35d22e2e7b3e301baf7f"
- decrypt 8 rounds:
string(32) "00000000000000000000000000000000"
string(32) "00000000000000000000000000000000"
- restart encryption:
string(32) "f097c0d167658edf4fbb3a9dd5ad2b71"
string(32) "f097c0d167658edf4fbb3a9dd5ad2b71"
- restart decryption:
string(32) "00000000000000000000000000000000"
string(32) "c923e03039e3df80e68edc2ef95a0cb5"
- large data:
int(10485760)
int(10485760)
