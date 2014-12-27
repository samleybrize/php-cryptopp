<?php

namespace Cryptopp;

interface MacInterface extends HashTransformationInterface
{
    /**
     * Sets the key
     * @param string $key
     * @throws \Cryptopp\CryptoppException
     */
    public function setKey($key);
}
