--TEST--
Symmetric transformation filter with user class
--FILE--
<?php

class SymmetricTransformationUser implements Cryptopp\SymmetricTransformationInterface
{
    private $key;
    private $iv;

    public function getName()
    {
        return "user";
    }

    public function getBlockSize()
    {
        return 5;
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

$c = new SymmetricTransformationUser();
$o = new Cryptopp\SymmetricTransformationFilter($c, new Cryptopp\PaddingPkcs7());
var_dump($o->getCipher()->getName());

// encryptString
echo "- encryptString:\n";
var_dump(bin2hex($o->encryptString("azertyuiop")));
var_dump(bin2hex($o->encryptString("azertyuiop")));
var_dump(bin2hex($o->encryptString("wxcvbnqsdfg")));

// decryptString
echo "- decryptString:\n";
var_dump($o->decryptString(hex2bin("706f6975797472657a610505050505")));
var_dump($o->decryptString(hex2bin("706f6975797472657a610505050505")));
var_dump($o->decryptString(hex2bin("666473716e62766378770404040467")));

// large data
echo "- large data:\n";
$data   = str_repeat("a", 10485760);
$pad    = str_repeat(chr(5), 5);
var_dump(strlen($o->encryptString($data)));
var_dump(strlen($o->decryptString($data . $pad)));

?>
--EXPECT--
string(4) "user"
- encryptString:
restarted string(30) "706f6975797472657a610505050505"
restarted string(30) "706f6975797472657a610505050505"
restarted string(30) "666473716e62766378770404040467"
- decryptString:
restarted string(10) "azertyuiop"
restarted string(10) "azertyuiop"
restarted string(11) "wxcvbnqsdfg"
- large data:
restarted int(10485765)
restarted int(10485760)
