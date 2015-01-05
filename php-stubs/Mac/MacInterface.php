<?php

namespace Cryptopp;

interface MacInterface extends HashTransformationInterface
{
    /**
     * Sets the key
     * @param string $key
     * @throws \Cryptopp\CryptoppException
     */
    public function setKey($key);

    /**
     * Indicates if a given key length is valid for this algorithm
     * @return bool
     */
    public function isValidKeyLength($keyLength);
}
