--TEST--
Hash algorithm: SHA3-256
--SKIPIF--
<?php
if (!class_exists("Cryptopp\HashSha3_256")) {
    echo "skip sha3 not available";
}
?>
--FILE--
<?php

var_dump(is_a("Cryptopp\HashSha3_256", "Cryptopp\HashAbstract", true));

// check algorithm infos
$o = new Cryptopp\HashSha3_256();
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
var_dump(in_array("sha3_256", Cryptopp\Hash::getAlgos()));
var_dump(Cryptopp\Hash::getClassname("sha3_256"));

?>
--EXPECT--
bool(true)
string(8) "sha3_256"
int(104)
int(32)
- digest calculation:
string(64) "caeaa437035747dc5931abb3cd05c0121d02e21c31c1867d01288bc9295f1365"
string(64) "113b396bb409b4193c303272b695cbbf9da205466dd3688a71ddc2a3966e784e"
- incremental hash:
string(64) "caeaa437035747dc5931abb3cd05c0121d02e21c31c1867d01288bc9295f1365"
- restart not necessary:
string(64) "caeaa437035747dc5931abb3cd05c0121d02e21c31c1867d01288bc9295f1365"
- restart:
string(64) "8d56d47093eebcbfdac9d6c72a241ea5100a73ca7d1ffb42bfd8668abdbed863"
- Cryptopp\Hash:
bool(true)
string(21) "Cryptopp\HashSha3_256"
