--TEST--
Hash transformation filter
--FILE--
<?php

$o = new Cryptopp\HashTransformationFilter(new Cryptopp\HashSha1());
var_dump($o->getHash()->getName());

// check final methods
echo "- final methods:\n";
$reflection = new ReflectionClass("Cryptopp\HashTransformationFilter");
$methods    = $reflection->getMethods();

foreach ($methods as $method) {
    if (!$method->isFinal()) {
        continue;
    }

    var_dump($method->getName());
}

// calculate digest
echo "- calculateDigestString:\n";
var_dump(bin2hex($o->calculateDigestString("azertyuiop")));
var_dump(bin2hex($o->calculateDigestString("azertyuiop")));
var_dump(bin2hex($o->calculateDigestString("wxcvbnqsdfg")));

// mac
echo "- mac:\n";
$m = new Cryptopp\MacHmac(new Cryptopp\HashSha1());
$o = new Cryptopp\HashTransformationFilter($m);
$m->setKey("1234567890123456");
var_dump(bin2hex($o->calculateDigestString("azertyuiop")));
var_dump(bin2hex($o->calculateDigestString("azertyuiop")));
var_dump(bin2hex($o->calculateDigestString("wxcvbnqsdfg")));

?>
--EXPECT--
string(4) "sha1"
- final methods:
string(7) "__sleep"
string(8) "__wakeup"
string(7) "getHash"
string(21) "calculateDigestString"
- calculateDigestString:
string(40) "58ad983135fe15c5a8e2e15fb5b501aedcf70dc2"
string(40) "58ad983135fe15c5a8e2e15fb5b501aedcf70dc2"
string(40) "0593a3f434eacdbf5c56faf578b2139ef91e3bc0"
- mac:
string(40) "8254f31ab6a9b0fae9a47205b71396ecc94775e5"
string(40) "8254f31ab6a9b0fae9a47205b71396ecc94775e5"
string(40) "2e9c4a1885c06bffe3c6e745cf688950e021c7e0"
