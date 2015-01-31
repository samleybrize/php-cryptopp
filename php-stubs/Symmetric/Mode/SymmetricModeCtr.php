<?php

namespace Cryptopp;

class SymmetricModeCtr extends SymmetricModeAbstract
{
    /**
     * Constructor
     * @param Cryptopp\BlockCipherInterface $cipher block cipher to use
     * @throws \Cryptopp\CryptoppException
     */
    public function __construct(BlockCipherInterface $cipher) {}
}
