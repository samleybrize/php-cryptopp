--TEST--
Cipher mode: CTR with user class (errors)
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
$o = new Cryptopp\SymmetricModeCtr($c);
$o->setKey(hex2bin("2b7e15"));

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
$o = new Cryptopp\SymmetricModeCtr(new BlockCipherUser());

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without iv
echo "- no iv:\n";
$o = new Cryptopp\SymmetricModeCtr(new BlockCipherUser());
$o->setKey(hex2bin("2b7e15"));

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
    $o = new Cryptopp\SymmetricModeCtr(new BlockCipherUser2());
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
- invalid key:
Cryptopp\SymmetricModeCtr : 33 is not a valid key length
Cryptopp\SymmetricModeCtr : a key is required
- invalid iv:
Cryptopp\SymmetricModeCtr : 3 is not a valid initialization vector length
- no key:
Cryptopp\SymmetricModeCtr : a key is required
- no iv:
Cryptopp\SymmetricModeCtr : an initialization vector is required
- block size 0:
BlockCipherUser2 : invalid block size returned
