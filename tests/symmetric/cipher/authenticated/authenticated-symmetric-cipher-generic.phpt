--TEST--
Authenticated symmetric cipher generic
--FILE--
<?php

var_dump(is_a("Cryptopp\AuthenticatedSymmetricCipherGeneric", "Cryptopp\AuthenticatedSymmetricCipherAbstract", true));

// check algorithm infos
$c = new Cryptopp\StreamCipherSosemanuk();
$m = new Cryptopp\MacHmac(new Cryptopp\HashSha1());
$o = new Cryptopp\AuthenticatedSymmetricCipherGeneric($c, $m);
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->getDigestSize());
var_dump($o->getCipher()->getName());
var_dump($o->getMac()->getName());

// encrypt
echo "- encrypt:\n";
$o->setMacKey(hex2bin("feffe9928665731c6d6a8f9467308308"));
$o->setKey(hex2bin("a7c083feb7"));
$o->setIv(hex2bin("00112233445566778899aabbccddeeff"));
var_dump(bin2hex($o->encrypt(hex2bin("00000000000000000000000000000000"))));
var_dump(bin2hex($o->encrypt(hex2bin("00000000000000000000000000000000"))));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt
echo "- decrypt:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("fe81d2162c9a100d04895c454a77515b"))));
var_dump(bin2hex($o->decrypt(hex2bin("be6a431a935cb90e2221ebb7ef502328"))));
var_dump(bin2hex($o->finalizeDecryption()));

// restart encryption
echo "- restart encryption:\n";
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("00000000000000000000000000000000"))));
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("00000000000000000000000000000000"))));
var_dump(bin2hex($o->finalizeEncryption()));

// restart decryption
echo "- restart decryption:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("fe81d2162c9a100d04895c454a77515b"))));
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("be6a431a935cb90e2221ebb7ef502328"))));
var_dump(bin2hex($o->finalizeDecryption()));

// encrypt data + aad
echo "- encrypt + aad:\n";
$o->restart();
$o->addEncryptionAdditionalData(hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2"));
$o->encrypt(hex2bin("00000000000000000000000000000000"));
$o->encrypt(hex2bin("00000000000000000000000000000000"));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt data + aad
echo "- decrypt + aad:\n";
$o->restart();
$o->addDecryptionAdditionalData(hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2"));
$o->decrypt(hex2bin("fe81d2162c9a100d04895c454a77515b"));
$o->decrypt(hex2bin("be6a431a935cb90e2221ebb7ef502328"));
var_dump(bin2hex($o->finalizeDecryption()));

// encrypt aad only
echo "- encrypt aad only:\n";
$o->setKey(hex2bin("77be637089"));
$o->setIv(hex2bin("e0e00f19fed7ba0136a797f3ed7ba013"));
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
string(20) "sosemanuk/hmac(sha1)"
int(1)
int(20)
string(9) "sosemanuk"
string(10) "hmac(sha1)"
- encrypt:
string(32) "fe81d2162c9a100d04895c454a77515b"
string(32) "be6a431a935cb90e2221ebb7ef502328"
string(40) "15cbfe20bb447711c2700b5eddada57323007973"
- decrypt:
string(32) "00000000000000000000000000000000"
string(32) "00000000000000000000000000000000"
string(40) "15cbfe20bb447711c2700b5eddada57323007973"
- restart encryption:
string(32) "fe81d2162c9a100d04895c454a77515b"
string(32) "fe81d2162c9a100d04895c454a77515b"
string(40) "9fc9456bb645743c404a85619a2f0f6fe754791b"
- restart decryption:
string(32) "00000000000000000000000000000000"
string(32) "40eb910cbfc6a90326a8b7f2a5277273"
string(40) "f044210a699da578851f2a700205218fc03a6e9d"
- encrypt + aad:
string(40) "ccc6f4a1ccb0c3e03a0b3e103613bef65d5f61de"
- decrypt + aad:
string(40) "ccc6f4a1ccb0c3e03a0b3e103613bef65d5f61de"
- encrypt aad only:
string(40) "02a2913e1a34d07005ebf2ba59a1008ba1f1307f"
- decrypt aad only:
string(40) "02a2913e1a34d07005ebf2ba59a1008ba1f1307f"
- large data:
int(10485760)
int(10485760)
