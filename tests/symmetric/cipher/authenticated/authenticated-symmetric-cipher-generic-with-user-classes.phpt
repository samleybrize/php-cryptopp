--TEST--
Authenticated symmetric cipher generic with user classes
--FILE--
<?php

// TODO check that key/iv are set properly
class SymmetricTransformationUser implements Cryptopp\SymmetricTransformationInterface
{
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
    }

    public function setIv($iv)
    {
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

// check algorithm infos
$c = new SymmetricTransformationUser();
$m = new MacUser();
$o = new Cryptopp\AuthenticatedSymmetricCipherGeneric($c, $m);
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->getDigestSize());
var_dump($o->getCipher()->getName());
var_dump($o->getMac()->getName());

// encrypt
echo "- encrypt:\n";
$o->setMacKey("123456789");
$o->setKey("123456");
$o->setIv("1234567");
var_dump($o->encrypt("azertyuiopqwerty"));
var_dump($o->encrypt("qsdfghjklmnbvcxw"));
var_dump($o->finalizeEncryption());

// decrypt
echo "- decrypt:\n";
$o->restart();
var_dump($o->decrypt("ytrewqpoiuytreza"));
var_dump($o->decrypt("wxcvbnmlkjhgfdsq"));
var_dump($o->finalizeDecryption());

// restart
echo "- restart:\n";
$o->restart();
var_dump($o->encrypt("azertyuiopqwerty"));
$o->restart();
var_dump($o->encrypt("azertyuiopqwerty"));
var_dump($o->finalizeEncryption());

// encrypt data + aad
echo "- encrypt + aad:\n";
$o->restart();
$o->addEncryptionAdditionalData("feedfacedead");
$o->encrypt("azertyuiopqwerty");
$o->encrypt("qsdfghjklmnbvcxw");
var_dump($o->finalizeEncryption());

// decrypt data + aad
echo "- decrypt + aad:\n";
$o->restart();
$o->addDecryptionAdditionalData("feedfacedead");
$o->decrypt("ytrewqpoiuytreza");
$o->decrypt("wxcvbnmlkjhgfdsq");
var_dump($o->finalizeDecryption());

// encrypt aad only
echo "- encrypt aad only:\n";
$o->addEncryptionAdditionalData("feedfacedead");
var_dump($o->finalizeEncryption());

// decrypt aad only
echo "- decrypt aad only:\n";
$o->restart();
$o->addDecryptionAdditionalData("feedfacedead");
var_dump($o->finalizeDecryption());

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
string(11) "userc/userm"
int(8)
int(10)
string(5) "userc"
string(5) "userm"
- encrypt:
restarted restarted restarted restarted restarted restarted string(16) "ytrewqpoiuytreza"
string(16) "wxcvbnmlkjhgfdsq"
restarted restarted restarted restarted string(10) "4582cb6203"
- decrypt:
restarted restarted restarted restarted string(16) "azertyuiopqwerty"
string(16) "qsdfghjklmnbvcxw"
restarted restarted restarted restarted string(10) "4582cb6203"
- restart:
restarted restarted restarted restarted string(16) "ytrewqpoiuytreza"
restarted restarted restarted restarted string(16) "ytrewqpoiuytreza"
restarted restarted restarted restarted string(10) "dd902ae895"
- encrypt + aad:
restarted restarted restarted restarted restarted restarted restarted restarted string(10) "18ce872628"
- decrypt + aad:
restarted restarted restarted restarted restarted restarted restarted restarted string(10) "18ce872628"
- encrypt aad only:
restarted restarted restarted restarted string(10) "d3b1071126"
- decrypt aad only:
restarted restarted restarted restarted restarted restarted restarted restarted string(10) "d3b1071126"
- invalid key:
Cryptopp\AuthenticatedSymmetricCipherGeneric : 33 is not a valid key length
Cryptopp\AuthenticatedSymmetricCipherGeneric : a key is required
- invalid iv:
Cryptopp\AuthenticatedSymmetricCipherGeneric : 3 is not a valid initialization vector length
Cryptopp\AuthenticatedSymmetricCipherGeneric : an initialization vector is required
- no key:
restarted restarted Cryptopp\AuthenticatedSymmetricCipherGeneric : a key is required
- no iv:
restarted restarted Cryptopp\AuthenticatedSymmetricCipherGeneric : an initialization vector is required
- no mac key:
restarted restarted restarted restarted Cryptopp\AuthenticatedSymmetricCipherGeneric : a MAC key is required
- encrypt before aad:
restarted restarted restarted restarted restarted restarted restarted restarted restarted restarted restarted restarted restarted restarted Cryptopp\AuthenticatedSymmetricCipherGeneric: additional authenticated data must be added before any encryption
- decrypt before aad:
Cryptopp\AuthenticatedSymmetricCipherGeneric: additional authenticated data must be added before any decryption
