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

class MacUser implements Cryptopp\MacInterface
{
    private $buffer = "";

    public function getName()
    {
        return "userm";
    }

    public function getDigestSize()
    {
        return 8;
    }

    public function getBlockSize()
    {
        return 5;
    }

    public function isValidKeyLength($l)
    {
        return 8 == $l;
    }

    public function setKey($key)
    {
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
$o->setMacKey("12345678");
$o->setKey("123456");
$o->setIv("12345");
var_dump(bin2hex($o->encrypt(hex2bin("00000000000000000000000000000000"))));
var_dump(bin2hex($o->encrypt(hex2bin("00000000000000000000000000000000"))));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt
echo "- decrypt:\n";
$o->restart();
var_dump(bin2hex($o->decrypt(hex2bin("fe81d2162c9a100d04895c454a77515b"))));
var_dump(bin2hex($o->decrypt(hex2bin("be6a431a935cb90e2221ebb7ef502328"))));
var_dump(bin2hex($o->finalizeDecryption()));

// restart
echo "- restart:\n";
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("00000000000000000000000000000000"))));
$o->restart();
var_dump(bin2hex($o->encrypt(hex2bin("00000000000000000000000000000000"))));
var_dump(bin2hex($o->finalizeEncryption()));

// encrypt data + aad
echo "- encrypt + aad:\n";
$o->restart();
$o->addEncryptionAdditionalData(hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2"));
$o->encrypt(hex2bin("00000000000000000000000000000000"));
$o->encrypt(hex2bin("00000000000000000000000000000000"));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt data + aad
echo "- decrypt + aad:\n";
$o->restart();
$o->addDecryptionAdditionalData(hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2"));
$o->decrypt(hex2bin("fe81d2162c9a100d04895c454a77515b"));
$o->decrypt(hex2bin("be6a431a935cb90e2221ebb7ef502328"));
var_dump(bin2hex($o->finalizeDecryption()));

// encrypt aad only
echo "- encrypt aad only:\n";
$o->addEncryptionAdditionalData(hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"));
var_dump(bin2hex($o->finalizeEncryption()));

// decrypt aad only
echo "- decrypt aad only:\n";
$o->restart();
$o->addDecryptionAdditionalData(hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"));
var_dump(bin2hex($o->finalizeDecryption()));

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
$o->setKey("12345");

try {
    $o->encrypt("123456");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// encrypt before aad
echo "- encrypt before aad:\n";
$o->restart();

$o->encrypt("azerty");

try {
    $o->addEncryptionAdditionalData("qwerty");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// decrypt before aad
echo "- decrypt before aad:\n";
$o->decrypt("azerty");

try {
    $o->addDecryptionAdditionalData("qwerty");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
string(11) "userc/userm"
int(5)
int(8)
string(5) "userc"
string(5) "userm"
- encrypt:
string(32) "fe81d2162c9a100d04895c454a77515b"
string(32) "be6a431a935cb90e2221ebb7ef502328"
string(40) "15cbfe20bb447711c2700b5eddada57323007973"
- decrypt:
string(32) "00000000000000000000000000000000"
string(32) "00000000000000000000000000000000"
string(40) "15cbfe20bb447711c2700b5eddada57323007973"
- restart:
string(32) "fe81d2162c9a100d04895c454a77515b"
string(32) "fe81d2162c9a100d04895c454a77515b"
string(40) "16bb821c7ba92cc135b931716b30410d13869fa8"
- encrypt + aad:
string(40) "ccc6f4a1ccb0c3e03a0b3e103613bef65d5f61de"
- decrypt + aad:
string(40) "ccc6f4a1ccb0c3e03a0b3e103613bef65d5f61de"
- encrypt aad only:
string(40) "02a2913e1a34d07005ebf2ba59a1008ba1f1307f"
- decrypt aad only:
string(40) "02a2913e1a34d07005ebf2ba59a1008ba1f1307f"
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
- encrypt before aad:
Cryptopp\AuthenticatedSymmetricCipherGeneric: additional authenticated data must be added before any encryption
- decrypt before aad:
Cryptopp\AuthenticatedSymmetricCipherGeneric: additional authenticated data must be added before any decryption
