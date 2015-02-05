<?php

namespace Cryptopp;

abstract class DigestUtils
{
    /**
     * Timing attack safe string comparison
     * @param string $known_string the string of known length to compare against
     * @param string $user_string the user-supplied string
     * @return boolean
     */
    public static function equals($known_string, $user_string) {}
}
