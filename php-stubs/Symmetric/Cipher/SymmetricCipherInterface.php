<?php

namespace Cryptopp;

// TODO comment
interface SymmetricCipherInterface
{
    // TODO comment
    public function getName() {}

    // TODO comment
    public function getBlockSize() {}

    // TODO comment
    public function isValidKeyLength($keyLength) {}

    // TODO comment
    public function setKey($key) {}
}
