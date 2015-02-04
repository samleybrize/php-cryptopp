--TEST--
Authenticated symmetric cipher: CCM
--FILE--
<?php

var_dump(is_a("Cryptopp\AuthenticatedSymmetricCipherCcm", "Cryptopp\AuthenticatedSymmetricCipherAbstract", true));

// check algorithm infos
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\AuthenticatedSymmetricCipherCcm($c);
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->getDigestSize());

// key length check
echo "- key length check:\n";
var_dump($o->isValidKeyLength(16));
var_dump($o->isValidKeyLength(15));
var_dump($o->isValidKeyLength(17));

// iv length check
echo "- iv length check:\n";
var_dump($o->isValidIvLength(0));
var_dump($o->isValidIvLength(2));
var_dump($o->isValidIvLength(7));
var_dump($o->isValidIvLength(13));
var_dump($o->isValidIvLength(15));

// set key
echo "- set key:\n";
$o->setKey("azertyuiopqsdfgh");
var_dump($o->getKey());
var_dump($c->getKey());

// set iv
echo "- set iv:\n";
$o->setIv("fghjklmazerty");
var_dump($o->getIv());

// encrypt
echo "- encrypt:\n";
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\AuthenticatedSymmetricCipherCcm($c);
$o->setIv(Cryptopp\HexUtils::hex2bin("00000003020100a0a1a2a3a4a5"));
$c->setKey(Cryptopp\HexUtils::hex2bin("c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"));
$o->setDigestSize(8);
$o->specifyDataSize(23, 0);
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("08090a0b0c0d0e0f101112"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("131415161718191a1b1c1d1e"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeEncryption()));

// decrypt
echo "- decrypt:\n";
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\AuthenticatedSymmetricCipherCcm($c);
$o->setIv(Cryptopp\HexUtils::hex2bin("00000003020100a0a1a2a3a4a5"));
$c->setKey(Cryptopp\HexUtils::hex2bin("c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"));
$o->setDigestSize(8);
$o->specifyDataSize(23, 0);
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("588c979a61c663d2f066d0"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("c2c0f989806d5f6b61dac384"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeDecryption()));

// restart encryption
echo "- restart encryption:\n";
$o->restart();
$o->setDigestSize(16);
$o->specifyDataSize(11, 0);
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("08090a0b0c0d0e0f101112"))));
$o->restart();
$o->setDigestSize(16);
$o->specifyDataSize(12, 0);
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("131415161718191a1b1c1d1e"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeEncryption()));

// restart decryption
echo "- restart decryption:\n";
$o->restart();
$o->setDigestSize(16);
$o->specifyDataSize(11, 0);
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("588c979a61c663d2f066d0"))));
$o->restart();
$o->setDigestSize(16);
$o->specifyDataSize(12, 0);
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("c2c0f989806d5f6b61dac384"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeDecryption()));

// encrypt data + aad
echo "- encrypt + aad:\n";
$o->restart();
$o->specifyDataSize(60, 20);
$o->setDigestSize(12);
$o->addEncryptionAdditionalData(Cryptopp\HexUtils::hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2"));
$o->encrypt(Cryptopp\HexUtils::hex2bin("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"));
$o->encrypt(Cryptopp\HexUtils::hex2bin("1c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39"));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeEncryption()));

// decrypt data + aad
echo "- decrypt + aad:\n";
$o->restart();
$o->specifyDataSize(60, 20);
$o->setDigestSize(12);
$o->addDecryptionAdditionalData(Cryptopp\HexUtils::hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2"));
$o->decrypt(Cryptopp\HexUtils::hex2bin("89b4afb4954f6b38452ecb147b19b90df3e1d829d3ea6d254a40ac3b545c87fd"));
$o->decrypt(Cryptopp\HexUtils::hex2bin("021e99bf795762d89d611c7bbe452d49ddde077fff1c34366f875232"));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeDecryption()));

// encrypt aad only
echo "- encrypt aad only:\n";
$o->setKey(Cryptopp\HexUtils::hex2bin("77be63708971c4e240d1cb79e8d77feb"));
$o->setIv(Cryptopp\HexUtils::hex2bin("e0e00f19fed7ba0136a797f3"));
$o->specifyDataSize(0, 16);
$o->setDigestSize(14);
$o->addEncryptionAdditionalData(Cryptopp\HexUtils::hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeEncryption()));

// decrypt aad only
echo "- decrypt aad only:\n";
$o->restart();
$o->specifyDataSize(0, 16);
$o->setDigestSize(14);
$o->addDecryptionAdditionalData(Cryptopp\HexUtils::hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeDecryption()));

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485760);
$o->specifyDataSize(10485760, 10485760);
$o->setDigestSize(14);
$o->addEncryptionAdditionalData($data);
$o->addDecryptionAdditionalData($data);
var_dump(strlen($o->encrypt($data)));
var_dump(strlen($o->decrypt($data)));

?>
--EXPECT--
bool(true)
string(8) "ccm(aes)"
int(1)
int(16)
- key length check:
bool(true)
bool(false)
bool(false)
- iv length check:
bool(false)
bool(false)
bool(true)
bool(true)
bool(false)
- set key:
string(16) "azertyuiopqsdfgh"
string(16) "azertyuiopqsdfgh"
- set iv:
string(13) "fghjklmazerty"
- encrypt:
string(22) "588c979a61c663d2f066d0"
string(24) "c2c0f989806d5f6b61dac384"
string(16) "7c2051a7ae200bcf"
- decrypt:
string(22) "08090a0b0c0d0e0f101112"
string(24) "131415161718191a1b1c1d1e"
string(16) "7c2051a7ae200bcf"
- restart encryption:
string(22) "588c979a61c663d2f066d0"
string(24) "439188877ad374c7fb6bdfcf"
string(32) "e8599abbfb0fa24247ceb346d0e68532"
- restart decryption:
string(22) "08090a0b0c0d0e0f101112"
string(24) "92456418eda632b681ad0155"
string(32) "dab641c7493a27b8127514bd5f423b23"
- encrypt + aad:
string(24) "fc738cbb02a4a7a8972fc20d"
- decrypt + aad:
string(24) "fc738cbb02a4a7a8972fc20d"
- encrypt aad only:
string(28) "b07790c26ada0c6b28576fc70234"
- decrypt aad only:
string(28) "b07790c26ada0c6b28576fc70234"
- large data:
int(10485760)
int(10485760)
