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
     * Returns the key
     * @return string
     */
    public function getKey();

    /**
     * Indicates if a given key length is valid for this algorithm
     * @return bool
     */
    public function isValidKeyLength($keyLength);
}
