--TEST--
Authenticated symmetric cipher: EAX
--FILE--
<?php

var_dump(is_a("Cryptopp\AuthenticatedSymmetricCipherEax", "Cryptopp\AuthenticatedSymmetricCipherAbstract", true));

// check algorithm infos
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\AuthenticatedSymmetricCipherEax($c);
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
var_dump($o->isValidIvLength(4));
var_dump($o->isValidIvLength(56));
var_dump($o->isValidIvLength(1256));

// set key
echo "- set key:\n";
$o->setKey("azertyuiopqsdfgh");
var_dump($o->getKey());
var_dump($c->getKey());

// set iv
echo "- set iv:\n";
$o->setIv("qsdfghjklmazerty");
var_dump($o->getIv());

// encrypt
echo "- encrypt:\n";
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\AuthenticatedSymmetricCipherEax($c);
$o->setIv(Cryptopp\HexUtils::hex2bin("cafebabefacedbaddecaf888"));
$c->setKey(Cryptopp\HexUtils::hex2bin("feffe9928665731c6d6a8f9467308308"));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("1c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b391aafd255"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeEncryption()));

// decrypt
echo "- decrypt:\n";
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\AuthenticatedSymmetricCipherEax($c);
$o->setIv(Cryptopp\HexUtils::hex2bin("cafebabefacedbaddecaf888"));
$c->setKey(Cryptopp\HexUtils::hex2bin("feffe9928665731c6d6a8f9467308308"));
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("206760741b9e5b75eaa69c401709f41b67b9b92d559953f31a892d17619f14ee"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("8ca8c56d81522be3831d4503b1903302079b7863c7e4d8869196996e6061c60a"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeDecryption()));

// restart encryption
echo "- restart encryption:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"))));
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeEncryption()));

// restart decryption
echo "- restart decryption:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("206760741b9e5b75eaa69c401709f41b67b9b92d559953f31a892d17619f14ee"))));
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("206760741b9e5b75eaa69c401709f41b67b9b92d559953f31a892d17619f14ee"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeDecryption()));

// encrypt data + aad
echo "- encrypt + aad:\n";
$o->restart();
$o->addEncryptionAdditionalData(Cryptopp\HexUtils::hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2"));
$o->encrypt(Cryptopp\HexUtils::hex2bin("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"));
$o->encrypt(Cryptopp\HexUtils::hex2bin("1c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39"));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeEncryption()));

// decrypt data + aad
echo "- decrypt + aad:\n";
$o->restart();
$o->addDecryptionAdditionalData(Cryptopp\HexUtils::hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2"));
$o->decrypt(Cryptopp\HexUtils::hex2bin("206760741b9e5b75eaa69c401709f41b67b9b92d559953f31a892d17619f14ee"));
$o->decrypt(Cryptopp\HexUtils::hex2bin("8ca8c56d81522be3831d4503b1903302079b7863c7e4d8869196996e"));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeDecryption()));

// encrypt aad only
echo "- encrypt aad only:\n";
$o->setKey(Cryptopp\HexUtils::hex2bin("77be63708971c4e240d1cb79e8d77feb"));
$o->setIv(Cryptopp\HexUtils::hex2bin("e0e00f19fed7ba0136a797f3"));
$o->addEncryptionAdditionalData(Cryptopp\HexUtils::hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeEncryption()));

// decrypt aad only
echo "- decrypt aad only:\n";
$o->restart();
$o->addDecryptionAdditionalData(Cryptopp\HexUtils::hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeDecryption()));

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485760);
$o->restart();
$o->addEncryptionAdditionalData($data);
$o->addDecryptionAdditionalData($data);
var_dump(strlen($o->encrypt($data)));
var_dump(strlen($o->decrypt($data)));

?>
--EXPECT--
bool(true)
string(8) "eax(aes)"
int(1)
int(16)
- key length check:
bool(true)
bool(false)
bool(false)
- iv length check:
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
- set key:
string(16) "azertyuiopqsdfgh"
string(16) "azertyuiopqsdfgh"
- set iv:
string(16) "qsdfghjklmazerty"
- encrypt:
string(64) "206760741b9e5b75eaa69c401709f41b67b9b92d559953f31a892d17619f14ee"
string(64) "8ca8c56d81522be3831d4503b1903302079b7863c7e4d8869196996e6061c60a"
string(32) "8480cf5fd94501ad47d870eafd4acdf4"
- decrypt:
string(64) "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"
string(64) "1c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b391aafd255"
string(32) "8480cf5fd94501ad47d870eafd4acdf4"
- restart encryption:
string(64) "206760741b9e5b75eaa69c401709f41b67b9b92d559953f31a892d17619f14ee"
string(64) "206760741b9e5b75eaa69c401709f41b67b9b92d559953f31a892d17619f14ee"
string(32) "c465d44f42020a050abb97ca55c83fc0"
- restart decryption:
string(64) "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"
string(64) "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"
string(32) "c465d44f42020a050abb97ca55c83fc0"
- encrypt + aad:
string(32) "86465458445f4cd43787dadcebb8ce97"
- decrypt + aad:
string(32) "86465458445f4cd43787dadcebb8ce97"
- encrypt aad only:
string(32) "315156e3ff92aba9ea818d18c08e6edc"
- decrypt aad only:
string(32) "315156e3ff92aba9ea818d18c08e6edc"
- large data:
int(10485760)
int(10485760)
