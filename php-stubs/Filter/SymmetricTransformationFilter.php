<?php

namespace Cryptopp;

class SymmetricTransformationFilter
{
    // TODO comment
    public function __construct(SymmetricTransformationInterface $cipher, PaddingInterface $padding = null) {}

    // TODO comment
    final public function getCipher() {}

    // TODO comment
    final public function getPadding() {}

    // TODO comment
    final public function encryptString($data) {}

    // TODO comment
    final public function decryptString($cipherText) {}
}
