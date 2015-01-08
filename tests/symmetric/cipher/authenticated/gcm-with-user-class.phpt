--TEST--
Authenticated symmetric cipher: GCM with user class
--FILE--
<?php

class BlockCipherUser implements Cryptopp\BlockCipherInterface
{
    private $key;

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

    public function getKey()
    {
        return $this->key;
    }
}

// check algorithm infos
$c = new BlockCipherUser();
$o = new Cryptopp\AuthenticatedSymmetricCipherGcm($c);
var_dump($o->getName());
var_dump($o->getBlockSize());

// key length check
echo "- key length check:\n";
var_dump($o->isValidKeyLength(3));
var_dump($o->isValidKeyLength(2));
var_dump($o->isValidKeyLength(4));

// iv length check
echo "- iv length check:\n";
var_dump($o->isValidIvLength(0));
var_dump($o->isValidIvLength(2));
var_dump($o->isValidIvLength(4));
var_dump($o->isValidIvLength(56));
var_dump($o->isValidIvLength(1256));

// set key
echo "- set key:\n";
$o->setKey(hex2bin("2b7e15"));
$o->setIv("1234567");
var_dump(bin2hex($c->getKey()));

// encrypt
echo "- encrypt:\n";
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e40"))));
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35c"))));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt
echo "- decrypt:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("556de8609b18"))));
var_dump(bin2hex($o->decrypt(hex2bin("5d45eda64796"))));
var_dump(bin2hex($o->finalizeDecryption()));

// restart encryption
echo "- restart encryption:\n";
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e40"))));
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35c"))));
var_dump(bin2hex($o->finalizeEncryption()));

// restart decryption
echo "- restart decryption:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("556de8609b18"))));
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("5d45eda64796"))));
var_dump(bin2hex($o->finalizeDecryption()));

// invalid key
echo "- invalid key:\n";
try {
    $o->setKey("123456789012345678901234567890123");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $o->setKey("");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without key
echo "- no key:\n";
$o = new Cryptopp\AuthenticatedSymmetricCipherGcm(new BlockCipherUser());

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// block size = 0
echo "- invalid block size:\n";
class BlockCipherUser2 extends BlockCipherUser
{
    public function getBlockSize()
    {
        return 8;
    }
}

try {
    $o = new Cryptopp\AuthenticatedSymmetricCipherGcm(new BlockCipherUser2());
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
string(9) "gcm(user)"
int(1)
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
string(6) "2b7e15"
- encrypt:
string(12) "556de8609b18"
string(12) "5d45eda64796"
string(32) "252755fd5f304d61cbc3aeb98b7a2274"
- decrypt:
string(12) "6bc1bee22e40"
string(12) "30c81c46a35c"
string(32) "252755fd5f304d61cbc3aeb98b7a2274"
- restart encryption:
string(12) "556de8609b18"
string(12) "0e644ac41604"
string(32) "ab7c3052893459107df39eb3291139b2"
- restart decryption:
string(12) "6bc1bee22e40"
string(12) "63e9bb24f2ce"
string(32) "0d0ea2ff5b784b98c48849998f2ef87e"
- invalid key:
Cryptopp\AuthenticatedSymmetricCipherGcm : 33 is not a valid key length
Cryptopp\AuthenticatedSymmetricCipherGcm : a key is required
- no key:
Cryptopp\AuthenticatedSymmetricCipherGcm : a key is required
- invalid block size:
Cryptopp\AuthenticatedSymmetricCipherGcm require a block cipher with a block size of 128 bits (16 bytes)
