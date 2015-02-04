--TEST--
Stream cipher: Panama
--FILE--
<?php

var_dump(is_a("Cryptopp\StreamCipherPanama", "Cryptopp\StreamCipherAbstract", true));

// check algorithm infos
$o = new Cryptopp\StreamCipherPanama();
var_dump($o->getName());
var_dump($o->getBlockSize());

// key length check
echo "- key length check:\n";
var_dump($o->isValidKeyLength(32));
var_dump($o->isValidKeyLength(23));
var_dump($o->isValidKeyLength(9));
var_dump($o->isValidKeyLength(0));
var_dump($o->isValidKeyLength(33));

// iv length check
echo "- iv length check:\n";
var_dump($o->isValidIvLength(32));
var_dump($o->isValidIvLength(15));
var_dump($o->isValidIvLength(33));

// set key
echo "- set key:\n";
$o->setKey("000102030405060708090a0b0c0d0e0f");
var_dump($o->getKey());

// set iv
echo "- set iv:\n";
$o->setIv("f07f5ff2ccd01a0a7d44acd6d239c2af");
var_dump($o->getIv());

// encrypt
echo "- encrypt:\n";
$o->setKey(Cryptopp\HexUtils::hex2bin("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"));
$o->setIv(Cryptopp\HexUtils::hex2bin("f07f5ff2ccd01a0a7d44acd6d239c2af0da1ff35275baf5dfa6e09411b79d8b9"));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));

// decrypt
echo "- decrypt:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("33d6c41cea88376420433bd3b7f88b27"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("506ace67d9841193058773d9b13b33b7"))));

// encrypt big endian
echo "- encrypt big endian:\n";
$o = new Cryptopp\StreamCipherPanama(Cryptopp\BIG_ENDIAN);
$o->setKey(Cryptopp\HexUtils::hex2bin("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"));
$o->setIv(Cryptopp\HexUtils::hex2bin("f07f5ff2ccd01a0a7d44acd6d239c2af0da1ff35275baf5dfa6e09411b79d8b9"));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));

// decrypt big endian
echo "- decrypt big endian:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("9df3d24ff46ccd8db521339fd4f788f8"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("bb2767e89d14ca669c8f7b311a8bef6b"))));

// restart encryption
echo "- restart encryption:\n";
$o = new Cryptopp\StreamCipherPanama(Cryptopp\LITTLE_ENDIAN);
$o->setKey(Cryptopp\HexUtils::hex2bin("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"));
$o->setIv(Cryptopp\HexUtils::hex2bin("f07f5ff2ccd01a0a7d44acd6d239c2af0da1ff35275baf5dfa6e09411b79d8b9"));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("00000000000000000000000000000000"))));

// restart decryption
echo "- restart decryption:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("33d6c41cea88376420433bd3b7f88b27"))));
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("506ace67d9841193058773d9b13b33b7"))));

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485760);
var_dump(strlen($o->encrypt($data)));
var_dump(strlen($o->decrypt($data)));

?>
--EXPECT--
bool(true)
string(6) "panama"
int(1)
- key length check:
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
- iv length check:
bool(true)
bool(false)
bool(false)
- set key:
string(32) "000102030405060708090a0b0c0d0e0f"
- set iv:
string(32) "f07f5ff2ccd01a0a7d44acd6d239c2af"
- encrypt:
string(32) "33d6c41cea88376420433bd3b7f88b27"
string(32) "506ace67d9841193058773d9b13b33b7"
- decrypt:
string(32) "00000000000000000000000000000000"
string(32) "00000000000000000000000000000000"
- encrypt big endian:
string(32) "9df3d24ff46ccd8db521339fd4f788f8"
string(32) "bb2767e89d14ca669c8f7b311a8bef6b"
- decrypt big endian:
string(32) "00000000000000000000000000000000"
string(32) "00000000000000000000000000000000"
- restart encryption:
string(32) "33d6c41cea88376420433bd3b7f88b27"
string(32) "33d6c41cea88376420433bd3b7f88b27"
- restart decryption:
string(32) "00000000000000000000000000000000"
string(32) "63bc0a7b330c26f725c4480a06c3b890"
- large data:
int(10485760)
int(10485760)
