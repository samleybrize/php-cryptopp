<?php

namespace Cryptopp;

interface HashTransformationInterface
{
    /**
     * Returns algorithm name
     * @return string
     */
    public function getName();

    /**
     * Returns digest size (in bytes)
     * @return int
     */
    public function getDigestSize();

    /**
     * Returns block size (in bytes)
     * @return int
     */
    public function getBlockSize();

    // TODO comment
    public function calculateDigest($data);

    // TODO comment
    public function update($data);

    // TODO comment
    public function finalize();

    // TODO comment
    public function restart();
}
