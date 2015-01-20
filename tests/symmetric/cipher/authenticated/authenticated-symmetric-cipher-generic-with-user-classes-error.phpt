--TEST--
Authenticated symmetric cipher generic with user classes (errors)
--FILE--
<?php

class SymmetricTransformationUser implements Cryptopp\SymmetricTransformationInterface
{
    private $key;
    private $iv;

    public function getName()
    {
        return "userc";
    }

    public function getBlockSize()
    {
        return 8;
    }

    public function isValidKeyLength($keyLength)
    {
        return 6 == $keyLength;
    }

    public function isValidIvLength($length)
    {
        return 7 == $length;
    }

    public function setKey($key)
    {
        $this->key = $key;
    }

    public function setIv($iv)
    {
        $this->iv = $iv;
    }

    public function getKey()
    {
        return $this->key;
    }

    public function getIv()
    {
        return $this->iv;
    }

    public function encrypt($data)
    {
        return strrev($data);
    }

    public function decrypt($data)
    {
        return strrev($data);
    }

    public function restart()
    {
        echo "restarted ";
    }
}

class MacUser implements Cryptopp\MacInterface
{
    private $key;
    private $buffer = "";

    public function getName()
    {
        return "userm";
    }

    public function getDigestSize()
    {
        return 10;
    }

    public function getBlockSize()
    {
        return 5;
    }

    public function isValidKeyLength($l)
    {
        return 9 == $l;
    }

    public function setKey($key)
    {
        $this->key = $key;
    }

    public function getKey()
    {
        return $this->key;
    }

    public function calculateDigest($data)
    {
        return substr(md5($data), 0, $this->getDigestSize());
    }

    public function update($data)
    {
        $this->buffer .= $data;
    }

    public function finalize()
    {
        return $this->calculateDigest($this->buffer);
    }

    public function restart()
    {
        $this->buffer = "";
    }
}

$c = new SymmetricTransformationUser();
$m = new MacUser();
$o = new Cryptopp\AuthenticatedSymmetricCipherGeneric($c, $m);
$o->setMacKey("123456789");
$o->setKey("123456");
$o->setIv("1234567");

echo ".\n";

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

try {
    $o->setIv("");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without key
echo "- no key:\n";
$c = new SymmetricTransformationUser();
$m = new MacUser();
$o = new Cryptopp\AuthenticatedSymmetricCipherGeneric($c, $m);
$o->setIv("1234567");
$o->setMacKey("123456789");

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without iv
echo "- no iv:\n";
$c = new SymmetricTransformationUser();
$m = new MacUser();
$o = new Cryptopp\AuthenticatedSymmetricCipherGeneric($c, $m);
$o->setKey("123456");
$o->setMacKey("123456789");

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt without mac key
echo "- no mac key:\n";
$c = new SymmetricTransformationUser();
$m = new MacUser();
$o = new Cryptopp\AuthenticatedSymmetricCipherGeneric($c, $m);
$o->setKey("123456");
$o->setIv("1234567");

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt before aad
echo "- encrypt before aad:\n";
$o->setKey("123456");
$o->setMacKey("123456789");
$o->setIv("1234567");
$o->restart();

$o->encrypt("azertyui");

try {
    $o->addEncryptionAdditionalData("qwerty");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// decrypt before aad
echo "- decrypt before aad:\n";
$o->decrypt("azertyui");

try {
    $o->addDecryptionAdditionalData("qwerty");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
restarted restarted .
- invalid key:
Cryptopp\AuthenticatedSymmetricCipherGeneric : 33 is not a valid key length
Cryptopp\AuthenticatedSymmetricCipherGeneric : a key is required
- invalid iv:
Cryptopp\AuthenticatedSymmetricCipherGeneric : 3 is not a valid initialization vector length
Cryptopp\AuthenticatedSymmetricCipherGeneric : an initialization vector is required
- no key:
Cryptopp\AuthenticatedSymmetricCipherGeneric : a key is required
- no iv:
Cryptopp\AuthenticatedSymmetricCipherGeneric : an initialization vector is required
- no mac key:
restarted restarted Cryptopp\AuthenticatedSymmetricCipherGeneric : a MAC key is required
- encrypt before aad:
restarted restarted restarted restarted restarted restarted Cryptopp\AuthenticatedSymmetricCipherGeneric: additional authenticated data must be added before any encryption
- decrypt before aad:
Cryptopp\AuthenticatedSymmetricCipherGeneric: additional authenticated data must be added before any decryption
