--TEST--
Symmetric transformation filter with user class
--FILE--
<?php

class SymmetricTransformationUser implements Cryptopp\SymmetricTransformationInterface
{
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
