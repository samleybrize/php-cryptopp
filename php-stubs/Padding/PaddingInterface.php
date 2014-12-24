<?php

namespace Cryptopp;

// TODO comment
interface PaddingInterface
{
    // TODO
    public function pad($data, $blockSize) {}

    // TODO
    public function unpad($data, $blockSize) {}

    // TODO
    public function canPad() {}

    // TODO
    public function canUnpad() {}
}
