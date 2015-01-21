--TEST--
Authenticated symmetric cipher: GCM
--FILE--
<?php

var_dump(is_a("Cryptopp\AuthenticatedSymmetricCipherGcm", "Cryptopp\AuthenticatedSymmetricCipherAbstract", true));

// check algorithm infos
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\AuthenticatedSymmetricCipherGcm($c);
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
$o = new Cryptopp\AuthenticatedSymmetricCipherGcm($c);
$o->setIv(hex2bin("cafebabefacedbaddecaf888"));
$c->setKey(hex2bin("feffe9928665731c6d6a8f9467308308"));
var_dump(bin2hex($o->encrypt(hex2bin("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"))));
var_dump(bin2hex($o->encrypt(hex2bin("1c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b391aafd255"))));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt
echo "- decrypt:\n";
$c = new Cryptopp\BlockCipherAes();
$o = new Cryptopp\AuthenticatedSymmetricCipherGcm($c);
$o->setIv(hex2bin("cafebabefacedbaddecaf888"));
$c->setKey(hex2bin("feffe9928665731c6d6a8f9467308308"));
var_dump(bin2hex($o->decrypt(hex2bin("42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e"))));
var_dump(bin2hex($o->decrypt(hex2bin("21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091473f5985"))));
var_dump(bin2hex($o->finalizeDecryption()));

// restart encryption
echo "- restart encryption:\n";
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"))));
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"))));
var_dump(bin2hex($o->finalizeEncryption()));

// restart decryption
echo "- restart decryption:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e"))));
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e"))));
var_dump(bin2hex($o->finalizeDecryption()));

// encrypt data + aad
echo "- encrypt + aad:\n";
$o->restart();
$o->addEncryptionAdditionalData(hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2"));
$o->encrypt(hex2bin("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"));
$o->encrypt(hex2bin("1c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39"));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt data + aad
echo "- decrypt + aad:\n";
$o->restart();
$o->addDecryptionAdditionalData(hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2"));
$o->decrypt(hex2bin("42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e"));
$o->decrypt(hex2bin("21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091"));
var_dump(bin2hex($o->finalizeDecryption()));

// encrypt aad only
echo "- encrypt aad only:\n";
$o->setKey(hex2bin("77be63708971c4e240d1cb79e8d77feb"));
$o->setIv(hex2bin("e0e00f19fed7ba0136a797f3"));
$o->addEncryptionAdditionalData(hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt aad only
echo "- decrypt aad only:\n";
$o->restart();
$o->addDecryptionAdditionalData(hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"));
var_dump(bin2hex($o->finalizeDecryption()));

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485760);
var_dump(strlen($o->encrypt($data)));
var_dump(strlen($o->decrypt($data)));

?>
--EXPECT--
bool(true)
string(8) "gcm(aes)"
int(1)
int(16)
- key length check:
bool(true)
bool(false)
bool(false)
- iv length check:
bool(false)
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
string(64) "42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e"
string(64) "21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091473f5985"
string(32) "4d5c2af327cd64a62cf35abd2ba6fab4"
- decrypt:
string(64) "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"
string(64) "1c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b391aafd255"
string(32) "4d5c2af327cd64a62cf35abd2ba6fab4"
- restart encryption:
string(64) "42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e"
string(64) "42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e"
string(32) "67383e3899332afdd4d83a204575a052"
- restart decryption:
string(64) "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"
string(64) "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"
string(32) "67383e3899332afdd4d83a204575a052"
- encrypt + aad:
string(32) "5bc94fbc3221a5db94fae95ae7121a47"
- decrypt + aad:
string(32) "5bc94fbc3221a5db94fae95ae7121a47"
- encrypt aad only:
string(32) "209fcc8d3675ed938e9c7166709dd946"
- decrypt aad only:
string(32) "209fcc8d3675ed938e9c7166709dd946"
- large data:
int(10485760)
int(10485760)
