<?php

namespace Cryptopp;

class HashTransformationFilter
{
    /**
     * Constructor
     * @param \Cryptopp\HashTransformationInterface $hash hash algorithm to use
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
     */
    final public function calculateDigestString($data) {}
}
