--TEST--
Authenticated symmetric cipher: EAX with user class
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

// check algorithm infos
$c = new BlockCipherUser();
$o = new Cryptopp\AuthenticatedSymmetricCipherEax($c);
var_dump($o->getName());
var_dump($o->getBlockSize());

// key length check
echo "- key length check:\n";
var_dump($o->isValidKeyLength(3));
var_dump($o->isValidKeyLength(2));
var_dump($o->isValidKeyLength(4));

// iv length check
echo "- iv length check:\n";
var_dump($o->isValidIvLength(0));
var_dump($o->isValidIvLength(2));
var_dump($o->isValidIvLength(4));
var_dump($o->isValidIvLength(56));
var_dump($o->isValidIvLength(1256));

// set key
echo "- set key:\n";
$o->setKey(Cryptopp\HexUtils::hex2bin("2b7e15"));
$o->setIv("1234567");
var_dump(Cryptopp\HexUtils::bin2hex($o->getKey()));
var_dump(Cryptopp\HexUtils::bin2hex($c->getKey()));

// encrypt
echo "- encrypt:\n";
$c = new BlockCipherUser();
$o = new Cryptopp\AuthenticatedSymmetricCipherEax($c);
$o->setIv("1234567");
$c->setKey(Cryptopp\HexUtils::hex2bin("2b7e15"));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("6bc1bee22e40"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("30c81c46a35c"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeEncryption()));

// decrypt
echo "- decrypt:\n";
$c = new BlockCipherUser();
$o = new Cryptopp\AuthenticatedSymmetricCipherEax($c);
$o->setIv("1234567");
$c->setKey(Cryptopp\HexUtils::hex2bin("2b7e15"));
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("a6a2211173dc"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("644b418b70eb"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeDecryption()));

// restart encryption
echo "- restart encryption:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("6bc1bee22e40"))));
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->encrypt(Cryptopp\HexUtils::hex2bin("30c81c46a35c"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeEncryption()));

// restart decryption
echo "- restart decryption:\n";
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("a6a2211173dc"))));
$o->restart();
var_dump(Cryptopp\HexUtils::bin2hex($o->decrypt(Cryptopp\HexUtils::hex2bin("644b418b70eb"))));
var_dump(Cryptopp\HexUtils::bin2hex($o->finalizeDecryption()));

// large data
echo "- large data:\n";
$data = str_repeat("a", 10485760);
$o->restart();
$o->addEncryptionAdditionalData($data);
$o->addDecryptionAdditionalData($data);
var_dump(strlen($o->encrypt($data)));
var_dump(strlen($o->decrypt($data)));

?>
--EXPECT--
string(9) "eax(user)"
int(1)
- key length check:
bool(true)
bool(false)
bool(false)
- iv length check:
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
- set key:
string(6) "2b7e15"
string(6) "2b7e15"
- encrypt:
string(12) "a6a2211173dc"
string(12) "644b418b70eb"
string(32) "73ffce5b1b6dacfd4419dbbbb366b5d5"
- decrypt:
string(12) "6bc1bee22e40"
string(12) "30c81c46a35c"
string(32) "73ffce5b1b6dacfd4419dbbbb366b5d5"
- restart encryption:
string(12) "a6a2211173dc"
string(12) "fdab83b5fec0"
string(32) "437ab12038f1df70c31d939df7e264d5"
- restart decryption:
string(12) "6bc1bee22e40"
string(12) "a928de782d77"
string(32) "46822d0d965662888b0d7416a9da86f8"
- large data:
int(10485760)
int(10485760)
