<?php

namespace Cryptopp;

class SymmetricTransformationFilter
{
    /**
     * Constructor
     * @param \Cryptopp\SymmetricTransformationInterface $cipher symmetric cipher to use
     * @param \Cryptopp\PaddingInterface $padding [optional] padding scheme to use. Defaults to Cryptopp\PaddingPkcs7.
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
     */
    final public function encryptString($data) {}

    /**
     * Decrypts data
     * @param string $cipherText data to decrypt
     * @return string decrypted data
     */
    final public function decryptString($cipherText) {}
}
