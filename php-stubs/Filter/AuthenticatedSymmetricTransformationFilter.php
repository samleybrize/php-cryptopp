<?php

namespace Cryptopp;

class AuthenticatedSymmetricTransformationFilter
{
    /**
     * Constructor
     * @param \Cryptopp\AuthenticatedSymmetricCipherInterface $cipher authenticated symmetric cipher to use
     * @param \Cryptopp\PaddingInterface $padding [optional] padding scheme to use. Defaults to Cryptopp\PaddingPkcs7.
     * @throws \Cryptopp\CryptoppException
     */
    public function __construct(AuthenticatedSymmetricCipherInterface $cipher, PaddingInterface $padding = null) {}

    /**
     * Returns the authenticated symmetric cipher used
     * @return \Cryptopp\AuthenticatedSymmetricCipherInterface
     */
    final public function getCipher() {}

    /**
     * Returns the padding scheme used
     * @return \Cryptopp\PaddingInterface
     */
    final public function getPadding() {}

    /**
     * Encrypts data.
     * Although $data and $aad are optional, at least one of them is required.
     * @param string $data [optional] data to encrypt
     * @param string $aad [optional] additional authenticated data
     * @return string encrypted data with the MAC tag appended
     * @throws \Cryptopp\CryptoppException
     */
    final public function encryptString($data = null, $aad = null) {}

    /**
     * Decrypts data.
     * Although $ciphertext and $aad are optional, at least one of them is required.
     * @param string $ciphertext [optional] data to decrypt. The MAC tag must be at the end.
     * @param string $aad [optional] additional authenticated data
     * @return string decrypted data
     * @throws \Cryptopp\CryptoppException
     * @throws \Cryptopp\CryptoppMacVerificationFailedException
     */
    final public function decryptString($ciphertext = null, $aad = null) {}

    /**
     * Disables object serialization
     */
    final public function __sleep() {}
    
    /**
     * Disables object serialization
     */
    final public function __wakeup() {}
}
