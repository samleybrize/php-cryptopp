<?php

namespace Cryptopp;

abstract class BlockCipher
{
    /**
     * Returns the list of supported block ciphers
     * @return array
     */
    public static function getAlgos() {}

    /**
     * Returns the name of the class that implements a block cipher
     * @param string $algoName algorithm name
     * @return string
     */
    public static function getClassname($algoName) {}
}
