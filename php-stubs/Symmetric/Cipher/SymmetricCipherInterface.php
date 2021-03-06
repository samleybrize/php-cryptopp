<?php

namespace Cryptopp;

interface SymmetricCipherInterface
{
    /**
     * Returns the cipher name
     * @return string
     */
    public function getName();

    /**
     * Returns cipher block size (in bytes)
     * @return int
     */
    public function getBlockSize();

    /**
     * Indicates if a given key length is valid for this algorithm
     * @param int $keyLength
     * @return bool
     */
    public function isValidKeyLength($keyLength);

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
}
