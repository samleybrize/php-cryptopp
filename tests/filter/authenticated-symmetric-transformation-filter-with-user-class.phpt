--TEST--
Authenticated symmetric transformation filter with user class
--FILE--
<?php

class AuthenticatedSymmetricCipherUser implements Cryptopp\AuthenticatedSymmetricCipherInterface
{
    private $bufferEncryption = "";
    private $bufferDecryption = "";
    private $key;
    private $iv;

    public function getName()
    {
        return "user";
    }

    public function getBlockSize()
    {
        return 1;
    }

    public function isValidKeyLength($keyLength)
    {
        return 6 == $keyLength;
    }

    public function isValidIvLength($keyLength)
    {
        return 7 == $keyLength;
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
        $data = strrev($data);
        $this->bufferEncryption .= $data;
        return $data;
    }

    public function decrypt($data)
    {
        $this->bufferDecryption .= $data;
        return strrev($data);
    }

    public function restart()
    {
        $this->bufferEncryption = "";
        $this->bufferDecryption = "";
    }

    public function getDigestSize()
    {
        return 8;
    }

    public function addEncryptionAdditionalData($data)
    {
        $this->bufferEncryption .= $data;
    }

    public function addDecryptionAdditionalData($data)
    {
        $this->bufferDecryption .= $data;
    }

    public function finalizeEncryption()
    {
        return substr(strrev($this->bufferEncryption), 0, $this->getDigestSize());
    }

    public function finalizeDecryption()
    {
        return substr(strrev($this->bufferDecryption), 0, $this->getDigestSize());
    }
}

$a = new AuthenticatedSymmetricCipherUser();
$o = new Cryptopp\AuthenticatedSymmetricTransformationFilter($a);
var_dump($o->getCipher()->getName());

// encryptString
echo "- encryptString:\n";
$a->setKey("1234567890123456");
$a->setIv("1234567890123456");
var_dump(bin2hex($o->encryptString("azertyuiop")));
var_dump(bin2hex($o->encryptString("azertyuiop")));
var_dump(bin2hex($o->encryptString("wxcvbnqsdfg")));

// decryptString
echo "- decryptString:\n";
var_dump($o->decryptString(hex2bin("706f6975797472657a61617a657274797569")));
var_dump($o->decryptString(hex2bin("706f6975797472657a61617a657274797569")));
var_dump($o->decryptString(hex2bin("67666473716e627663787777786376626e7173")));

// encrypt data + aad
echo "- encrypt + aad:\n";
$a->setKey(hex2bin("feffe9928665731c6d6a8f9467308308"));
$a->setIv(hex2bin("cafebabefacedbaddecaf888"));
$encrypted = $o->encryptString(
    hex2bin("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39"),
    hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2")
);
var_dump(bin2hex($encrypted));

// decrypt data + aad
echo "- decrypt + aad:\n";
$decrypted = $o->decryptString(
    hex2bin("397b63ba57e60daaf5ed6ab125b5a649240ecf2f53096895950c3c1c728a318a3d304c2edaf7341553a9a7869a26f5afc50959a5e50684f8253231d9d9313225f88406e5"),
    hex2bin("feedfacedeadbeeffeedfacedeadbeefabaddad2")
);
var_dump(bin2hex($decrypted));

// encrypt aad only
echo "- encrypt aad only:\n";
$a->setKey(hex2bin("77be63708971c4e240d1cb79e8d77feb"));
$a->setIv(hex2bin("e0e00f19fed7ba0136a797f3"));
var_dump(bin2hex($o->encryptString(null, hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"))));

// decrypt aad only
echo "- decrypt aad only:\n";
var_dump(bin2hex($o->decryptString(hex2bin("ab3c21a63365b1a0"), hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"))));

// mac verification failed
echo "- mac verification failed:\n";
try {
    $o->decryptString("12345678901234567890123456789012", hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"));
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
string(4) "user"
- encryptString:
string(36) "706f6975797472657a61617a657274797569"
string(36) "706f6975797472657a61617a657274797569"
string(38) "67666473716e627663787777786376626e7173"
- decryptString:
string(10) "azertyuiop"
string(10) "azertyuiop"
string(11) "wxcvbnqsdfg"
- encrypt + aad:
string(136) "397b63ba57e60daaf5ed6ab125b5a649240ecf2f53096895950c3c1c728a318a3d304c2edaf7341553a9a7869a26f5afc50959a5e50684f8253231d9d9313225f88406e5"
- decrypt + aad:
string(120) "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39"
- encrypt aad only:
string(16) "ab3c21a63365b1a0"
- decrypt aad only:
string(0) ""
- mac verification failed:
Cryptopp\AuthenticatedSymmetricTransformationFilter : MAC verification failed
