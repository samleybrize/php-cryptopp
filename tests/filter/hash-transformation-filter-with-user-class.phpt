--TEST--
Hash transformation filter with user class
--FILE--
<?php

class HashTransformationUser implements Cryptopp\HashTransformationInterface
{
    private $buffer = "";

    public function getName()
    {
        return "user";
    }

    public function getDigestSize()
    {
        return 8;
    }

    public function getBlockSize()
    {
        return 5;
    }

    public function calculateDigest($data)
    {
        return substr(strrev($data), 0, $this->getDigestSize());
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

$c = new HashTransformationUser();
$o = new Cryptopp\HashTransformationFilter($c);
var_dump($o->getHash()->getName());

// calculateDigestString
echo "- calculateDigestString:\n";
var_dump($o->calculateDigestString("azertyuiop"));
var_dump($o->calculateDigestString("azertyuiop"));
var_dump($o->calculateDigestString("wxcvbnqsdfg"));

// digest size = 0
echo "- digest size 0:\n";
class HashTransformationUser2 extends HashTransformationUser
{
    public function getDigestSize()
    {
        return 0;
    }
}

try {
    $o = new Cryptopp\HashTransformationFilter(new HashTransformationUser2());
    echo "digest size 0 ok\n";
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// bad returned digest size
echo "- bad returned digest size:\n";
class HashTransformationUser3 extends HashTransformationUser
{
    public function calculateDigest($data)
    {
        return strrev($data);
    }
}

try {
    $o = new Cryptopp\HashTransformationFilter(new HashTransformationUser3());
    $o->calculateDigestString("azertyuiop");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
string(4) "user"
- calculateDigestString:
string(8) "poiuytre"
string(8) "poiuytre"
string(8) "gfdsqnbv"
- digest size 0:
digest size 0 ok
- bad returned digest size:
HashTransformationUser3 : digest size is 8 bytes, returned 10 bytes
