<?php

namespace Cryptopp;

abstract class AuthenticatedSymmetricCipherAbstract implements AuthenticatedSymmetricCipherInterface
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
     * {@inheritdoc}
     */
    final public function getDigestSize() {}

    /**
     * {@inheritdoc}
     */
    final public function addEncryptionAdditionalData($data) {}

    /**
     * {@inheritdoc}
     */
    final public function addDecryptionAdditionalData($data) {}

    /**
     * {@inheritdoc}
     */
    final public function finalizeEncryption() {}

    /**
     * {@inheritdoc}
     */
    final public function finalizeDecryption() {}

    /**
     * Disables object serialization
     */
    final public function __sleep() {}
    
    /**
     * Disables object serialization
     */
    final public function __wakeup() {}
}
