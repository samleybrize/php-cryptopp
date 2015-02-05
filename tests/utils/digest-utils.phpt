--TEST--
DigestUtils
--FILE--
<?php

// equals
var_dump(Cryptopp\DigestUtils::equals("same", "same"));
var_dump(Cryptopp\DigestUtils::equals("not1same", "not2same"));
var_dump(Cryptopp\DigestUtils::equals("short", "longer"));
var_dump(Cryptopp\DigestUtils::equals("longer", "short"));
var_dump(Cryptopp\DigestUtils::equals("", "notempty"));
var_dump(Cryptopp\DigestUtils::equals("notempty", ""));
var_dump(Cryptopp\DigestUtils::equals("", ""));
var_dump(Cryptopp\DigestUtils::equals(123, "NaN"));
var_dump(Cryptopp\DigestUtils::equals("NaN", 123));
var_dump(Cryptopp\DigestUtils::equals(123, 123));
var_dump(Cryptopp\DigestUtils::equals(null, ""));
var_dump(Cryptopp\DigestUtils::equals(null, 123));
var_dump(Cryptopp\DigestUtils::equals(null, null));

?>
--EXPECTF--
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(true)

Warning: Cryptopp\DigestUtils::equals(): Expected known_string to be a string, integer given in %s on line %d
bool(false)

Warning: Cryptopp\DigestUtils::equals(): Expected user_string to be a string, integer given in %s on line %d
bool(false)

Warning: Cryptopp\DigestUtils::equals(): Expected known_string to be a string, integer given in %s on line %d
bool(false)

Warning: Cryptopp\DigestUtils::equals(): Expected known_string to be a string, null given in %s on line %d
bool(false)

Warning: Cryptopp\DigestUtils::equals(): Expected known_string to be a string, null given in %s on line %d
bool(false)

Warning: Cryptopp\DigestUtils::equals(): Expected known_string to be a string, null given in %s on line %d
bool(false)
