--TEST--
Authenticated symmetric cipher: CCM with user class
--FILE--
<?php

class BlockCipherUser implements Cryptopp\BlockCipherInterface
{
    private $key = "";

    public function getName()
    {
        return "user";
    }

    public function getBlockSize()
    {
        return 16;
    }

    public function isValidKeyLength($keyLength)
    {
        return 3 == $keyLength;
    }

    public function setKey($key)
    {
        $this->key = (string) $key;
    }

    public function getKey()
    {
        return $this->key;
    }

    public function encryptBlock($data)
    {
        return md5($data, true);
    }

    public function decryptBlock($data)
    {
        return md5($data, true);
    }

    public function encrypt($data)
    {
        return md5($data, true);
    }

    public function decrypt($data)
    {
        return md5($data, true);
    }
}

// check algorithm infos
$c = new BlockCipherUser();
$o = new Cryptopp\AuthenticatedSymmetricCipherCcm($c);
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->getDigestSize());

// key length check
echo "- key length check:\n";
var_dump($o->isValidKeyLength(3));
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
$o->setKey("aze");
var_dump($o->getKey());
var_dump($c->getKey());

// set iv
echo "- set iv:\n";
$o->setIv("fghjklmazerty");
var_dump($o->getIv());

// encrypt
echo "- encrypt:\n";
$c = new BlockCipherUser();
$o = new Cryptopp\AuthenticatedSymmetricCipherCcm($c);
$o->setIv(hex2bin("00000003020100a0a1a2a3a4a5"));
$c->setKey(hex2bin("c0c1c2"));
$o->setDigestSize(8);
$o->specifyDataSize(23, 0);
var_dump(bin2hex($o->encrypt(hex2bin("08090a0b0c0d0e0f101112"))));
var_dump(bin2hex($o->encrypt(hex2bin("131415161718191a1b1c1d1e"))));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt
echo "- decrypt:\n";
$c = new BlockCipherUser();
$o = new Cryptopp\AuthenticatedSymmetricCipherCcm($c);
$o->setIv(hex2bin("00000003020100a0a1a2a3a4a5"));
$c->setKey(hex2bin("c0c1c2"));
$o->setDigestSize(8);
$o->specifyDataSize(23, 0);
var_dump(bin2hex($o->decrypt(hex2bin("588c979a61c663d2f066d0"))));
var_dump(bin2hex($o->decrypt(hex2bin("c2c0f989806d5f6b61dac384"))));
var_dump(bin2hex($o->finalizeDecryption()));

// restart encryption
echo "- restart encryption:\n";
$o->restart();
$o->setDigestSize(16);
$o->specifyDataSize(11, 0);
var_dump(bin2hex($o->encrypt(hex2bin("08090a0b0c0d0e0f101112"))));
$o->restart();
$o->setDigestSize(16);
$o->specifyDataSize(12, 0);
var_dump(bin2hex($o->encrypt(hex2bin("131415161718191a1b1c1d1e"))));
var_dump(bin2hex($o->finalizeEncryption()));

// restart decryption
echo "- restart decryption:\n";
$o->restart();
$o->setDigestSize(16);
$o->specifyDataSize(11, 0);
var_dump(bin2hex($o->decrypt(hex2bin("588c979a61c663d2f066d0"))));
$o->restart();
$o->setDigestSize(16);
$o->specifyDataSize(12, 0);
var_dump(bin2hex($o->decrypt(hex2bin("c2c0f989806d5f6b61dac384"))));
var_dump(bin2hex($o->finalizeDecryption()));

// encrypt data + aad
echo "- encrypt + aad:\n";
$o->restart();
$o->specifyDataSize(60, 20);
$o->setDigestSize(12);
$o->addEncryptionAdditionalData(hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2"));
$o->encrypt(hex2bin("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"));
$o->encrypt(hex2bin("1c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39"));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt data + aad
echo "- decrypt + aad:\n";
$o->restart();
$o->specifyDataSize(60, 20);
$o->setDigestSize(12);
$o->addDecryptionAdditionalData(hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2"));
$o->decrypt(hex2bin("89b4afb4954f6b38452ecb147b19b90df3e1d829d3ea6d254a40ac3b545c87fd"));
$o->decrypt(hex2bin("021e99bf795762d89d611c7bbe452d49ddde077fff1c34366f875232"));
var_dump(bin2hex($o->finalizeDecryption()));

// encrypt aad only
echo "- encrypt aad only:\n";
$o->setKey(hex2bin("77be63"));
$o->setIv(hex2bin("e0e00f19fed7ba0136a797f3"));
$o->specifyDataSize(0, 16);
$o->setDigestSize(14);
$o->addEncryptionAdditionalData(hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt aad only
echo "- decrypt aad only:\n";
$o->restart();
$o->specifyDataSize(0, 16);
$o->setDigestSize(14);
$o->addDecryptionAdditionalData(hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"));
var_dump(bin2hex($o->finalizeDecryption()));

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
string(9) "ccm(user)"
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
string(3) "aze"
string(3) "aze"
- set iv:
string(13) "fghjklmazerty"
- encrypt:
string(22) "b08965cb1348f79934b740"
string(24) "e12511501497dcea32c7d30b"
string(16) "60b326f28b2cccaa"
- decrypt:
string(22) "e00cf85a7e839a44d4c082"
string(24) "30f1fdcf83e29a9b48010d91"
string(16) "221003c57f02dac0"
- restart encryption:
string(22) "b08965cb1348f79934b740"
string(24) "ab947ad6085de08c3fba4fec"
string(32) "c94a5f19037106aba3a5b4a6e79441f5"
- restart decryption:
string(22) "e00cf85a7e839a44d4c082"
string(24) "7a4096499f28a6fd457c9176"
string(32) "e3180084182a568435b2f0868b780b02"
- encrypt + aad:
string(24) "1e0023143324e7050044500a"
- decrypt + aad:
string(24) "a2c182d0e3b478bee1b14402"
- encrypt aad only:
string(28) "b814127074987363b1eb0d39dde6"
- decrypt aad only:
string(28) "b814127074987363b1eb0d39dde6"
- large data:
int(10485760)
int(10485760)
