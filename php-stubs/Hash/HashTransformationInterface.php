<?php

namespace Cryptopp;

// TODO comment
interface HashTransformationInterface
{
    // TODO comment
    public function getName() {}

    // TODO comment
    public function getDigestSize() {}

    // TODO comment
    public function getBlockSize() {}

    // TODO comment
    public function calculateDigest($data) {}

    // TODO comment
    public function update($data) {}

    // TODO comment
    public function finalize() {}

    // TODO comment
    public function restart() {}
}
