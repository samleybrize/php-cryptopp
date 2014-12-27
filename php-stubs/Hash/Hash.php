<?php

namespace Cryptopp;

abstract class Hash
{
    /**
     * Returns the list of supported hash algorithms
     * @return array
     */
    public static function getAlgos() {}

    /**
     * Returns the name of the class that implements a hash algorithm
     * @param string $algoName algorithm name
     * @return string
     */
    public static function getClassname($algoName) {}
}
