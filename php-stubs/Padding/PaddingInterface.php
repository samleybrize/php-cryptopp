<?php

namespace Cryptopp;

interface PaddingInterface
{
    /**
     * Pads data
     * @param string $data data to pad
     * @param int $blockSize number of bytes on which align data
     * @return string padded data
     * @throws \Cryptopp\CryptoppException
     */
    public function pad($data, $blockSize);

    /**
     * Unpads data
     * @param string $data data to unpad
     * @param int $blockSize number of bytes on which data is aligned
     * @return string unpadded data
     * @throws \Cryptopp\CryptoppException
     */
    public function unpad($data, $blockSize);

    /**
     * Indicates if this padding scheme can pad data
     * @return bool
     */
    public function canPad();

    /**
     * Indicates if this padding scheme can unpad data
     * @return bool
     */
    public function canUnpad();
}
