<?php

namespace Cryptopp;

abstract class Mac
{
    /**
     * Returns the list of supported MAC algorithms
     * @return array
     */
    public static function getAlgos() {}

    /**
     * Returns the name of the class that implements a MAC algorithm
     * @param string $algoName algorithm name
     * @return string
     */
    public static function getClassname($algoName) {}
}
