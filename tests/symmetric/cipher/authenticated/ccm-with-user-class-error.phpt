--TEST--
Authenticated symmetric cipher: CCM wih user class (errors)
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

$c = new BlockCipherUser();
$o = new Cryptopp\AuthenticatedSymmetricCipherCcm($c);
$o->setKey(hex2bin("77be63"));
$o->setIv(hex2bin("e0e00f19fed7ba0136a797f3"));

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
$o = new Cryptopp\AuthenticatedSymmetricCipherCcm(new BlockCipherUser());

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
- invalid key:
Cryptopp\AuthenticatedSymmetricCipherCcm : 33 is not a valid key length
Cryptopp\AuthenticatedSymmetricCipherCcm : a key is required
- no key:
Cryptopp\AuthenticatedSymmetricCipherCcm : a key is required
- invalid block size:
Cryptopp\AuthenticatedSymmetricCipherGcm require a block cipher with a block size of 128 bits (16 bytes)
