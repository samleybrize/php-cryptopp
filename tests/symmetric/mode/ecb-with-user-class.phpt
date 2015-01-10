--TEST--
Cipher mode: ECB with user class
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
$o = new Cryptopp\SymmetricModeEcb($c);
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

// set key
echo "- set key:\n";
$o->setKey(hex2bin("2b7e15"));
var_dump(bin2hex($o->getKey()));
var_dump(bin2hex($c->getKey()));

// encrypt
echo "- encrypt:\n";
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e40"))));
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35c"))));

// decrypt
echo "- decrypt:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("402ee2bec16b"))));
var_dump(bin2hex($o->decrypt(hex2bin("5ca3461cc830"))));

// restart encryption
echo "- restart encryption:\n";
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("6bc1bee22e40"))));
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("30c81c46a35c"))));

// restart decryption
echo "- restart decryption:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("402ee2bec16b"))));
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("5ca3461cc830"))));

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

// invalid iv
echo "- invalid iv:\n";
try {
    $o->setIv("123");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without key
echo "- no key:\n";
$o = new Cryptopp\SymmetricModeEcb(new BlockCipherUser());

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// block size = 0
echo "- block size 0:\n";
class BlockCipherUser2 extends BlockCipherUser
{
    public function getBlockSize()
    {
        return 0;
    }
}

try {
    $o = new Cryptopp\SymmetricModeEcb(new BlockCipherUser2());
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
string(9) "ecb(user)"
int(6)
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
string(12) "402ee2bec16b"
string(12) "5ca3461cc830"
- decrypt:
string(12) "6bc1bee22e40"
string(12) "30c81c46a35c"
- restart encryption:
string(12) "402ee2bec16b"
string(12) "5ca3461cc830"
- restart decryption:
string(12) "6bc1bee22e40"
string(12) "30c81c46a35c"
- invalid key:
Cryptopp\SymmetricModeEcb : 33 is not a valid key length
Cryptopp\SymmetricModeEcb : a key is required
- invalid iv:
Cryptopp\SymmetricModeEcb : no initialization vector needed
- no key:
Cryptopp\SymmetricModeEcb : a key is required
- block size 0:
Cryptopp\SymmetricModeAbstract can only be used with a block cipher with a block size greater than 0
