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

    // TODO comment
    public function isValidKeyLength($keyLength);

    // TODO comment
    public function setKey($key);
}
