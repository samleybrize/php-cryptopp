<?php

namespace Cryptopp;

abstract class BlockCipherAbstract implements BlockCipherInterface
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
    final public function setKey($key) {}

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
    final public function encryptBlock($block) {}

    /**
     * {@inheritdoc}
     */
    final public function decryptBlock($block) {}
}
