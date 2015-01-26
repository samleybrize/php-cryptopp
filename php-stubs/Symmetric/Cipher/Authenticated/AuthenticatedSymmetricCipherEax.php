<?php

namespace Cryptopp;

class AuthenticatedSymmetricCipherEax extends AuthenticatedSymmetricCipherAbstract
{
    /**
     * Constructor
     * @param \Cryptopp\BlockCipherInterface $cipher block cipher to use
     * @throws \Cryptopp\CryptoppException
     */
    public function __construct(BlockCipherInterface $cipher) {}
}
