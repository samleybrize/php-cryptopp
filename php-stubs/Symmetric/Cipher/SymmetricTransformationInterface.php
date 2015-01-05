<?php

namespace Cryptopp;

interface SymmetricTransformationInterface extends SymmetricCipherInterface
{
    /**
     * Sets the IV (initialization vector)
     * @param string $iv
     * @throws \Cryptopp\CryptoppException
     */
    public function setIv($iv);

    /**
     * Indicates if a given iv length is valid for this algorithm
     * @return bool
     */
    public function isValidIvLength($length);

    /**
     * Encrypts data
     * @param string $data data to encrypt
     * @return string encrypted data
     * @throws \Cryptopp\CryptoppException
     */
    public function encrypt($data);

    /**
     * Decrypts data
     * @param string $data data to decrypt
     * @return string decrypted data
     * @throws \Cryptopp\CryptoppException
     */
    public function decrypt($data);

    /**
     * Resets encryption/decryption state
     */
    public function restart();
}
