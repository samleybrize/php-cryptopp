<?php

namespace Cryptopp;

class MacCmac extends MacAbstract
{
    /**
     * Constructor
     * @param \Cryptopp\BlockCipherInterface $cipher cipher to use
     * @throws \Cryptopp\CryptoppException
     */
    public function __construct(BlockCipherInterface $cipher) {}
}
