--TEST--
Hash algorithm: SHA3-512
--SKIPIF--
<?php
if (!class_exists("Cryptopp\HashSha3_512")) {
    echo "skip sha3 not available";
}
?>
--FILE--
<?php

var_dump(is_a("Cryptopp\HashSha3_512", "Cryptopp\HashAbstract", true));

// check algorithm infos
$o = new Cryptopp\HashSha3_512();
var_dump($o->getName());
var_dump($o->getBlockSize());
var_dump($o->getDigestSize());

// check digest calculation
echo "- digest calculation:\n";
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

// check values returned by Cryptopp\Hash for this algorithm
echo "- Cryptopp\Hash:\n";
var_dump(in_array("sha3_512", Cryptopp\Hash::getAlgos()));
var_dump(Cryptopp\Hash::getClassname("sha3_512"));

?>
--EXPECT--
bool(true)
string(8) "sha3_512"
int(144)
int(64)
- digest calculation:
string(128) "a11d37ec4d9a4d6030e01faafdf75c4ba7537968e1debd5163fa57f44a847bbc96c8b193b2eb2dc29fac7c661976ede60728e4d794c12e97dac6869063ce252f"
string(128) "f2cf847e80859f5a67a726eb3fb333ae882fc1f2f8bfaa5fdb939c9143bc4849a229bf435e95e57f23aea2e1845c263e4288673a18f4d1123c92da6b954904b3"
- incremental hash:
string(128) "a11d37ec4d9a4d6030e01faafdf75c4ba7537968e1debd5163fa57f44a847bbc96c8b193b2eb2dc29fac7c661976ede60728e4d794c12e97dac6869063ce252f"
- restart not necessary:
string(128) "a11d37ec4d9a4d6030e01faafdf75c4ba7537968e1debd5163fa57f44a847bbc96c8b193b2eb2dc29fac7c661976ede60728e4d794c12e97dac6869063ce252f"
- restart:
string(128) "bca0f72b90de6b3a335a09814ec1d478d7d087f88ca2813f571b25728f1fb1c1bd8828a970294128490e59b2b7eeacec079ae0ebfbd9df7cc18cce27d23d516f"
- Cryptopp\Hash:
bool(true)
string(21) "Cryptopp\HashSha3_512"
