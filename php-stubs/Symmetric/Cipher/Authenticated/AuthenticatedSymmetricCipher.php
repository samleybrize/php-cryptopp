<?php

namespace Cryptopp;

abstract class AuthenticatedSymmetricCipher
{
    /**
     * Returns the list of supported authenticated symmetric ciphers
     * @return array
     */
    public static function getAlgos() {}

    /**
     * Returns the name of the class that implements an authenticated symmetric cipher
     * @param string $algoName algorithm name
     * @return string
     */
    public static function getClassname($algoName) {}
}
