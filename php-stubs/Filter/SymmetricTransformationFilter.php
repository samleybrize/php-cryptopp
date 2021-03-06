<?php

namespace Cryptopp;

class SymmetricTransformationFilter
{
    /**
     * Constructor
     * @param \Cryptopp\SymmetricTransformationInterface $cipher symmetric cipher to use
     * @param \Cryptopp\PaddingInterface $padding [optional] padding scheme to use. Defaults to Cryptopp\PaddingPkcs7.
     * @throws \Cryptopp\CryptoppException
     */
    public function __construct(SymmetricTransformationInterface $cipher, PaddingInterface $padding = null) {}

    /**
     * Returns the symmetric cipher used
     * @return \Cryptopp\SymmetricTransformationInterface
     */
    final public function getCipher() {}

    /**
     * Returns the padding scheme used
     * @return \Cryptopp\PaddingInterface
     */
    final public function getPadding() {}

    /**
     * Encrypts data
     * @param string $data data to encrypt
     * @return string encrypted data
     * @throws \Cryptopp\CryptoppException
     */
    final public function encryptString($data) {}

    /**
     * Decrypts data
     * @param string $ciphertext data to decrypt
     * @return string decrypted data
     * @throws \Cryptopp\CryptoppException
     */
    final public function decryptString($ciphertext) {}

    /**
     * Disables object serialization
     */
    final public function __sleep() {}

    /**
     * Disables object serialization
     */
    final public function __wakeup() {}
}
