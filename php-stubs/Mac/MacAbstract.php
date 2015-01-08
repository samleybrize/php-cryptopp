<?php

namespace Cryptopp;

abstract class MacAbstract implements MacInterface
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
    final public function isValidKeyLength($keyLength) {}

    /**
     * {@inheritdoc}
     */
    final public function setKey($key) {}

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
