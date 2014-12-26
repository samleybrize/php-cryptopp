<?php

namespace Cryptopp;

interface MacInterface extends HashTransformationInterface
{
    /**
     * Sets the key
     * @param string $key
     */
    public function setKey($key);
}
