<?php

namespace Cryptopp;

interface SymmetricTransformationInterface extends SymmetricCipherInterface
{
    /**
     * Sets the IV (initialization vector)
     * @param string $iv
     */
    public function setIv($iv);

    /**
     * Encrypts data
     * @param string $data data to encrypt
     * @return string encrypted data
     */
    public function encrypt($data);

    /**
     * Decrypts data
     * @param string $data data to decrypt
     * @return string decrypted data
     */
    public function decrypt($data);

    /**
     * Resets encryption/decryption state
     */
    public function restart();
}
