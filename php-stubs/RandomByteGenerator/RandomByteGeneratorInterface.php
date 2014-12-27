<?php

namespace Cryptopp;

interface RandomByteGeneratorInterface
{
    /**
     * Generates a random byte sequence
     * @param int $size size of the sequence (in bytes)
     * @return string
     * @throws \Cryptopp\CryptoppException
     */
    public function generate($size);
}
