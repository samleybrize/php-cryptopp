<?php

namespace Cryptopp;

abstract class SymmetricMode
{
    /**
     * Returns the list of supported symmetric modes
     * @return array
     */
    public static function getModes() {}

    /**
     * Returns the name of the class that implements a symmetric mode
     * @param string $modeName mode name
     * @return string
     */
    public static function getClassname($modeName) {}
}
