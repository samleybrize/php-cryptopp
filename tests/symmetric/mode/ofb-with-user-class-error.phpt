--TEST--
Cipher mode: OFB with user class (errors)
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

$c = new BlockCipherUser();
$o = new Cryptopp\SymmetricModeOfb($c);
$o->setKey(Cryptopp\HexUtils::hex2bin("2b7e15"));

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
$o = new Cryptopp\SymmetricModeOfb(new BlockCipherUser());

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without iv
echo "- no iv:\n";
$o = new Cryptopp\SymmetricModeOfb(new BlockCipherUser());
$o->setKey(Cryptopp\HexUtils::hex2bin("2b7e15"));

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
    $o = new Cryptopp\SymmetricModeOfb(new BlockCipherUser2());
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
- invalid key:
Cryptopp\SymmetricModeOfb : 33 is not a valid key length
Cryptopp\SymmetricModeOfb : a key is required
- invalid iv:
Cryptopp\SymmetricModeOfb : 3 is not a valid initialization vector length
- no key:
Cryptopp\SymmetricModeOfb : a key is required
- no iv:
Cryptopp\SymmetricModeOfb : an initialization vector is required
- block size 0:
BlockCipherUser2 : invalid block size returned
