<?php

namespace Cryptopp;

abstract class StreamCipher
{
    /**
     * Returns the list of supported stream ciphers
     * @return array
     */
    public static function getAlgos() {}

    /**
     * Returns the name of the class that implements a stream cipher
     * @param string $algoName algorithm name
     * @return string
     */
    public static function getClassname($algoName) {}
}
