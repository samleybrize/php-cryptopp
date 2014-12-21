--TEST--
MAC algorithm: HMAC with user class
--FILE--
<?php

class HashUser implements Cryptopp\HashInterface
{
    private $buffer = "";

    public function getName()
    {
        return "user";
    }

    public function getDigestSize()
    {
        return 12;
    }

    public function getBlockSize()
    {
        return 16;
    }

    public function calculateDigest($data)
    {
        $digestSize = $this->getDigestSize();
        $data       = strrev($data);
        $length     = strlen($data);

        if ($length > $digestSize) {
            $data = substr($data, 0, $digestSize);
        } elseif ($length < $digestSize) {
            $data = str_pad($data, $digestSize, "0", STR_PAD_RIGHT);
        }

        return $data;
    }

    public function update($data)
    {
        $this->buffer .= (string) $data;
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
$o = new Cryptopp\MacHmac(new HashUser());
var_dump($o->getName());
var_dump($o->getDigestSize());
var_dump($o->getBlockSize());

// check digest calculation
echo "- calculate digest:\n";
$o->setKey("123456");
var_dump(bin2hex($o->calculateDigest("qwertyuiop")));
var_dump(bin2hex($o->calculateDigest("azerty")));

// check incremental hash
echo "- incremental hash:\n";
$o->update("qwerty");
$o->update("uio");
$o->update("p");
var_dump(bin2hex($o->finalize()));

// check that a restart() is not necessary after a call to finalize()
echo "- restart not necessary:\n";
$o->update("qwerty");
$o->update("uio");
$o->update("p");
var_dump(bin2hex($o->finalize()));

// check restart()
echo "- restart:\n";
$o->update("qwerty");
$o->restart();
$o->update("uio");
$o->update("p");
var_dump(bin2hex($o->finalize()));

// hash algorithm with block size = 0
echo "- block size 0:\n";
class HashUser2 extends HashUser
{
    public function getBlockSize()
    {
        return 0;
    }
}

try {
    $o = new Cryptopp\MacHmac(new HashUser2());
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// hash algorithm with block size < digest size
echo "- block size < digest size:\n";
class HashUser3 extends HashUser
{
    public function getBlockSize()
    {
        return 2;
    }
}

try {
    $o = new Cryptopp\MacHmac(new HashUser3());
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// bad digest size returned
echo "- bad digest size:\n";
class HashUser4 extends HashUser
{
    public function calculateDigest($data)
    {
        $data = parent::calculateDigest($data);
        return $data . "1";
    }
}

try {
    $o = new Cryptopp\MacHmac(new HashUser4());
    $o->calculateDigest("qwertyuiop");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
string(10) "hmac(user)"
int(12)
int(0)
- calculate digest:
string(24) "363671776572747975696f70"
string(24) "363636363636617a65727479"
- incremental hash:
string(24) "363671776572747975696f70"
- restart not necessary:
string(24) "363671776572747975696f70"
- restart:
string(24) "363636363636363675696f70"
- block size 0:
Cryptopp\MacHmac can only be used with a block-based hash function (block size > 0)
- block size < digest size:
Cryptopp\MacHmac: hash block size (2) cannot be lower than digest size (12)
- bad digest size:
HashUser4 : digest size is 12 bytes, returned 13 bytes
