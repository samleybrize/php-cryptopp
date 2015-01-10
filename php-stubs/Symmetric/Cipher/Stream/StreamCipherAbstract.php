<?php

namespace Cryptopp;

abstract class StreamCipherAbstract implements StreamCipherInterface
{
    /**
     * {@inheritdoc}
     */
    final public function getName() {}

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
    final public function isValidIvLength($length) {}

    /**
     * {@inheritdoc}
     */
    final public function setKey($key) {}

    /**
     * {@inheritdoc}
     */
    final public function setIv($iv) {}

    /**
     * {@inheritdoc}
     */
    final public function getKey() {}

    /**
     * {@inheritdoc}
     */
    final public function getIv() {}

    /**
     * {@inheritdoc}
     */
    final public function encrypt($data) {}

    /**
     * {@inheritdoc}
     */
    final public function decrypt($data) {}

    /**
     * {@inheritdoc}
     */
    final public function restart() {}

    /**
     * Disables object serialization
     */
    final public function __sleep() {}
    
    /**
     * Disables object serialization
     */
    final public function __wakeup() {}
}
