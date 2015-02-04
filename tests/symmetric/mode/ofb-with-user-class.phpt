--TEST--
Cipher mode: OFB with user class
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
        return 6;
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
        return strrev($data);
    }

    public function decryptBlock($data)
    {
        return strrev($data);
    }

    public function encrypt($data)
    {
        return strrev($data);
    }

    public function decrypt($data)
    {
        return strrev($data);
    }

    public function getKey()
    {
        return $this->key;
    }
}

// check algorithm infos
$c = new BlockCipherUser();
$o = new Cryptopp\SymmetricModeOfb($c);
var_dump($o->getName());
var_dump($o->getBlockSize());

// key length check
echo "- key length check:\n";
var_dump($o->isValidKeyLength(3));
var_dump($o->isValidKeyLength(2));
var_dump($o->isValidKeyLength(4));

// iv length check
echo "- iv length check:\n";
var_dump($o->isValidIvLength(6));
var_dump($o->isValidIvLength(2));
var_dump($o->isValidIvLength(24));

// set key
echo "- set key:\n";
$o->setKey(Cryptopp\HexUtils::hex2bin("2b7e15"));
var_dump(Cryptopp\HexUtils::bin2hex($o->getKey()));
var_dump(Cryptopp\HexUtils::bin2hex($c->getKey()));

// encrypt
echo "- encrypt:\n";
$o->setIv(Cryptopp\HexUtils::hex2bin("3f457b32af65"));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("6bc1bee22e40"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("30c81c46a35c"))));

// decrypt
echo "- decrypt:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("2581d0c58454"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("082783cc4915"))));

// restart encryption
echo "- restart encryption:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("6bc1bee22e40"))));
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("30c81c46a35c"))));

// restart decryption
echo "- restart decryption:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("2581d0c58454"))));
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("082783cc4915"))));

// large data
echo "- large data:\n";
$data = str_repeat("a", 1747626);
var_dump(strlen($o->encrypt($data)));
var_dump(strlen($o->decrypt($data)));

?>
--EXPECT--
string(9) "ofb(user)"
int(1)
- key length check:
bool(true)
bool(false)
bool(false)
- iv length check:
bool(true)
bool(false)
bool(false)
- set key:
string(6) "2b7e15"
string(6) "2b7e15"
- encrypt:
string(12) "0e6e8c996b7f"
string(12) "0f8d67740c39"
- decrypt:
string(12) "402ee2bec16b"
string(12) "3762f8fee670"
- restart encryption:
string(12) "0e6e8c996b7f"
string(12) "55672e3de663"
- restart decryption:
string(12) "402ee2bec16b"
string(12) "6d88b1b70c2a"
- large data:
int(1747626)
int(1747626)
