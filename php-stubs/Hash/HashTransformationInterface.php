<?php

namespace Cryptopp;

interface HashTransformationInterface
{
    /**
     * Returns algorithm name
     * @return string
     */
    public function getName();

    /**
     * Returns digest size (in bytes)
     * @return int
     */
    public function getDigestSize();

    /**
     * Returns block size (in bytes)
     * @return int
     */
    public function getBlockSize();

    /**
     * Calculate the digest of a given string
     * @param string $data data for which to calculate the digest
     * @return string digest
     */
    public function calculateDigest($data);

    /**
     * Adds data to current incremental digest calculation
     * @param string $data data to add to current digest calculation
     */
    public function update($data);

    /**
     * Finalize current incremental digest calculation and return the resulting digest
     * @return string digest
     */
    public function finalize();

    /**
     * Resets current incremental digest calculation
     */
    public function restart();
}
