<?php

namespace Cryptopp;

class HashTransformationFilter
{
    /**
     * Constructor
     * @param \Cryptopp\HashTransformationInterface $hash hash algorithm to use
     * @throws \Cryptopp\CryptoppException
     */
    public function __construct(HashTransformationInterface $hash) {}

    /**
     * Returns the hash algorithm used
     * @return \Cryptopp\HashTransformationInterface
     */
    final public function getHash() {}

    /**
     * Calculate the digest of a given string
     * @param string $data data for which to calculate the digest
     * @return string digest
     * @throws \Cryptopp\CryptoppException
     */
    final public function calculateDigestString($data) {}

    /**
     * Disables object serialization
     */
    final public function __sleep() {}
    
    /**
     * Disables object serialization
     */
    final public function __wakeup() {}
}
