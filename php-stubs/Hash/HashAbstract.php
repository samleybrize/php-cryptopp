<?php

namespace Cryptopp;

// TODO comment
abstract class HashAbstract implements HashInterface
{
    /**
     * {@inheritdoc}
     */
    final public function getName() {}

    /**
     * {@inheritdoc}
     */
    final public function getDigestSize() {}

    /**
     * {@inheritdoc}
     */
    final public function getBlockSize() {}

    /**
     * {@inheritdoc}
     */
    final public function calculateDigest($data) {}

    /**
     * {@inheritdoc}
     */
    final public function update($data) {}

    /**
     * {@inheritdoc}
     */
    final public function finalize() {}

    /**
     * {@inheritdoc}
     */
    final public function restart() {}
}
