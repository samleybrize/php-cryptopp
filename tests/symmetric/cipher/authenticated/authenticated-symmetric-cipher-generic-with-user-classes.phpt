--TEST--
Authenticated symmetric cipher generic with user classes
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

// check algorithm infos
$c = new SymmetricTransformationUser();
$m = new MacUser();
$o = new Cryptopp\AuthenticatedSymmetricCipherGeneric($c, $m);
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->getDigestSize());
var_dump($o->getCipher()->getName());
var_dump($o->getMac()->getName());

// check that key/iv are set properly
echo "- key/iv:\n";
$o->setMacKey("123456789");
$o->setKey("123456");
$o->setIv("1234567");
var_dump($c->getKey());
var_dump($c->getIv());
var_dump($m->getKey());

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

// restart encryption
echo "- restart encryption:\n";
$o->restart();
var_dump($o->encrypt("azertyuiopqwerty"));
$o->restart();
var_dump($o->encrypt("azertyuiopqwerty"));
var_dump($o->finalizeEncryption());

// restart decryption
echo "- restart decryption:\n";
$o->restart();
var_dump($o->decrypt("ytrewqpoiuytreza"));
$o->restart();
var_dump($o->decrypt("ytrewqpoiuytreza"));
var_dump($o->finalizeDecryption());

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

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485760);
var_dump(strlen($o->encrypt($data)));
var_dump(strlen($o->decrypt($data)));

?>
--EXPECT--
string(11) "userc/userm"
int(8)
int(10)
string(5) "userc"
string(5) "userm"
- key/iv:
restarted restarted string(6) "123456"
string(7) "1234567"
string(9) "123456789"
- encrypt:
restarted restarted restarted restarted string(16) "ytrewqpoiuytreza"
string(16) "wxcvbnmlkjhgfdsq"
restarted restarted string(10) "4582cb6203"
- decrypt:
restarted restarted string(16) "azertyuiopqwerty"
string(16) "qsdfghjklmnbvcxw"
restarted restarted string(10) "4582cb6203"
- restart encryption:
restarted restarted string(16) "ytrewqpoiuytreza"
restarted restarted string(16) "ytrewqpoiuytreza"
restarted restarted string(10) "dd902ae895"
- restart decryption:
restarted restarted string(16) "azertyuiopqwerty"
restarted restarted string(16) "azertyuiopqwerty"
restarted restarted string(10) "dd902ae895"
- encrypt + aad:
restarted restarted restarted restarted string(10) "18ce872628"
- decrypt + aad:
restarted restarted restarted restarted string(10) "18ce872628"
- encrypt aad only:
restarted restarted string(10) "d3b1071126"
- decrypt aad only:
restarted restarted restarted restarted string(10) "d3b1071126"
- large data:
int(10485760)
int(10485760)
