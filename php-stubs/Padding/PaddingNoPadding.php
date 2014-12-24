<?php

namespace Cryptopp;

// TODO comment
class PaddingNoPadding implements PaddingInterface
{
    /**
     * {@inheritdoc}
     */
    public function pad($data, $blockSize) {}

    /**
     * {@inheritdoc}
     */
    public function unpad($data, $blockSize) {}

    /**
     * {@inheritdoc}
     */
    public function canPad() {}

    /**
     * {@inheritdoc}
     */
    public function canUnpad() {}
}
