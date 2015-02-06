--TEST--
MAC algorithm: CMAC with user class
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

// check algorithm infos
$c = new BlockCipherUser();
$o = new Cryptopp\MacCmac($c);
var_dump($o->getName());
var_dump($o->getDigestSize());
var_dump($o->getBlockSize());

// key length check
echo "- key length check:\n";
var_dump($o->isValidKeyLength(3));
var_dump($o->isValidKeyLength(16));
var_dump($o->isValidKeyLength(23));
var_dump($o->isValidKeyLength(125));
var_dump($o->isValidKeyLength(0));

// check digest calculation
echo "- calculate digest:\n";
$o->setKey("123");
var_dump(Cryptopp\HexUtils::bin2hex($o->calculateDigest("qwertyuiop")));
var_dump(Cryptopp\HexUtils::bin2hex($o->calculateDigest("azerty")));

// check incremental hash
echo "- incremental hash:\n";
$o->update("qwerty");
$o->update("uio");
$o->update("p");
var_dump(Cryptopp\HexUtils::bin2hex($o->finalize()));

// check that a restart() is not necessary after a call to finalize()
echo "- restart not necessary:\n";
$o->update("qwerty");
$o->update("uio");
$o->update("p");
var_dump(Cryptopp\HexUtils::bin2hex($o->finalize()));

// check restart()
echo "- restart:\n";
$o->update("qwerty");
$o->restart();
$o->update("uio");
$o->update("p");
var_dump(Cryptopp\HexUtils::bin2hex($o->finalize()));

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485760);
var_dump(strlen($o->calculateDigest($data)));
$o->update($data);
var_dump(strlen($o->finalize()));

// hash algorithm with block size = 0
echo "- block size 0:\n";
class BlockCipherUser2 extends BlockCipherUser
{
    public function getBlockSize()
    {
        return 0;
    }
}

try {
    $o = new Cryptopp\MacCmac(new BlockCipherUser2());
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
string(10) "cmac(user)"
int(6)
int(0)
- key length check:
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
- calculate digest:
string(12) "71f7151d1d0c"
string(12) "797472657a61"
- incremental hash:
string(12) "71f7151d1d0c"
- restart not necessary:
string(12) "71f7151d1d0c"
- restart:
string(12) "0080706f6975"
- large data:
int(6)
int(6)
- block size 0:
BlockCipherUser2 : invalid block size returned
