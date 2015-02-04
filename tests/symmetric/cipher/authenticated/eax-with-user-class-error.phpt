--TEST--
Authenticated symmetric cipher: EAX with user class (errors)
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
$o = new Cryptopp\AuthenticatedSymmetricCipherEax($c);
$o->setIv("1234567");
$c->setKey(Cryptopp\HexUtils::hex2bin("2b7e15"));

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
echo "- encrypt no key:\n";
$o = new Cryptopp\AuthenticatedSymmetricCipherEax(new BlockCipherUser());

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// aad without key
echo "- aad no key:\n";
$o = new Cryptopp\AuthenticatedSymmetricCipherEax(new BlockCipherUser());

try {
    $o->addEncryptionAdditionalData("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// empty iv
echo "- empty iv:\n";
$o = new Cryptopp\AuthenticatedSymmetricCipherEax(new BlockCipherUser());
$o->setKey("123");

try {
    $o->encrypt("123456");
    echo "empty iv ok\n";
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
    $o = new Cryptopp\AuthenticatedSymmetricCipherEax(new BlockCipherUser2());
    echo "block size 8 ok\n";
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
- invalid key:
Cryptopp\AuthenticatedSymmetricCipherEax : 33 is not a valid key length
Cryptopp\AuthenticatedSymmetricCipherEax : a key is required
- encrypt no key:
Cryptopp\AuthenticatedSymmetricCipherEax : a key is required
- aad no key:
Cryptopp\AuthenticatedSymmetricCipherEax : a key is required
- empty iv:
empty iv ok
- invalid block size:
block size 8 ok
