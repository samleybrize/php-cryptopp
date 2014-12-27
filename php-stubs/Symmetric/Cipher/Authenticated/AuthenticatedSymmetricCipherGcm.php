<?php

namespace Cryptopp;

class AuthenticatedSymmetricCipherGcm extends AuthenticatedSymmetricCipherAbstract
{
    /**
     * Constructor
     * @param \Cryptopp\BlockCipherInterface $cipher block cipher to use. Must be a 128 bits (16 bytes) block size cipher.
     * @throws \Cryptopp\CryptoppException
     */
    public function __construct(BlockCipherInterface $cipher) {}
}
