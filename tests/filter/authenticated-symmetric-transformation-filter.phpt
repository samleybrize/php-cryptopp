--TEST--
Authenticated symmetric transformation filter
--FILE--
<?php

$c = new Cryptopp\BlockCipherAes();
$a = new Cryptopp\AuthenticatedSymmetricCipherGcm($c);
$o = new Cryptopp\AuthenticatedSymmetricTransformationFilter($a);
var_dump($o->getCipher()->getName());

// check default padding
var_dump(get_class($o->getPadding()));

// check final methods
echo "- final methods:\n";
$reflection = new ReflectionClass("Cryptopp\AuthenticatedSymmetricTransformationFilter");
$methods    = $reflection->getMethods();

foreach ($methods as $method) {
    if (!$method->isFinal()) {
        continue;
    }

    var_dump($method->getName());
}

// encryptString
echo "- encryptString:\n";
$a->setKey("1234567890123456");
$a->setIv("1234567890123456");
var_dump(bin2hex($o->encryptString("azertyuiop")));
var_dump(bin2hex($o->encryptString("azertyuiop")));
var_dump(bin2hex($o->encryptString("wxcvbnqsdfg")));

// decryptString
echo "- decryptString:\n";
var_dump($o->decryptString(hex2bin("e7de80cef314ceaefb14dba4b8556d8ff3068cf910d9c537543b")));
var_dump($o->decryptString(hex2bin("e7de80cef314ceaefb14dba4b8556d8ff3068cf910d9c537543b")));
var_dump($o->decryptString(hex2bin("f1dc86cae503cab4f002de0bf33abe92ce878e120e8d82d971c75a")));

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
    hex2bin("42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e0915bc94fbc3221a5db94fae95ae7121a47"),
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
var_dump(bin2hex($o->decryptString(hex2bin("209fcc8d3675ed938e9c7166709dd946"), hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"))));

// mac verification failed
echo "- mac verification failed:\n";
try {
    $o->decryptString("12345678901234567890123456789012", hex2bin("7a43ec1d9c0a5a78a0b16533a6213cab"));
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// sleep
echo "- sleep:\n";
try {
    serialize($o);
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// bad arguments
echo "- bad arguments:\n";
var_dump(@$o->encryptString(array()));
echo "$php_errormsg\n";
var_dump(@$o->encryptString("a", array()));
echo "$php_errormsg\n";
var_dump(@$o->decryptString(array()));
echo "$php_errormsg\n";
var_dump(@$o->decryptString("e", array()));
echo "$php_errormsg\n";

// mode object: constructor not called
echo "- cipher object (parent constructor not called):\n";
class AuthenticatedChild extends Cryptopp\AuthenticatedSymmetricCipherGcm
{
    public function __construct(){}
}

try {
    $o = new Cryptopp\AuthenticatedSymmetricTransformationFilter(new AuthenticatedChild());
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

// test inheritance - parent constructor not called
echo "- inheritance (parent constructor not called):\n";
class ChildParentConstructorNotCalled extends Cryptopp\AuthenticatedSymmetricTransformationFilter
{
    public function __construct(){}
}

$o = new ChildParentConstructorNotCalled();

try {
    $o->encryptString("");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

try {
    $o->decryptString("");
} catch (Cryptopp\CryptoppException $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
string(8) "gcm(aes)"
string(21) "Cryptopp\PaddingPkcs7"
- final methods:
string(7) "__sleep"
string(8) "__wakeup"
string(9) "getCipher"
string(10) "getPadding"
string(13) "encryptString"
string(13) "decryptString"
- encryptString:
string(52) "e7de80cef314ceaefb14dba4b8556d8ff3068cf910d9c537543b"
string(52) "e7de80cef314ceaefb14dba4b8556d8ff3068cf910d9c537543b"
string(54) "f1dc86cae503cab4f002de0bf33abe92ce878e120e8d82d971c75a"
- decryptString:
string(10) "azertyuiop"
string(10) "azertyuiop"
string(11) "wxcvbnqsdfg"
- encrypt + aad:
string(152) "42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e0915bc94fbc3221a5db94fae95ae7121a47"
- decrypt + aad:
string(120) "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39"
- encrypt aad only:
string(32) "209fcc8d3675ed938e9c7166709dd946"
- decrypt aad only:
string(0) ""
- mac verification failed:
Cryptopp\AuthenticatedSymmetricTransformationFilter : MAC verification failed
- sleep:
You cannot serialize or unserialize Cryptopp\AuthenticatedSymmetricTransformationFilter instances
- bad arguments:
NULL
Cryptopp\AuthenticatedSymmetricTransformationFilter::encryptString() expects parameter 1 to be string, array given
NULL
Cryptopp\AuthenticatedSymmetricTransformationFilter::encryptString() expects parameter 2 to be string, array given
NULL
Cryptopp\AuthenticatedSymmetricTransformationFilter::decryptString() expects parameter 1 to be string, array given
NULL
Cryptopp\AuthenticatedSymmetricTransformationFilter::decryptString() expects parameter 2 to be string, array given
- cipher object (parent constructor not called):
AuthenticatedChild : parent constructor was not called
- inheritance (parent constructor not called):
Cryptopp\AuthenticatedSymmetricTransformationFilter : constructor was not called
Cryptopp\AuthenticatedSymmetricTransformationFilter : constructor was not called
