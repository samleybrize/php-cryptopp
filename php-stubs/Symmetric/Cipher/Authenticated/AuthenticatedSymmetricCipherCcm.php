<?php

namespace Cryptopp;

class AuthenticatedSymmetricCipherCcm extends AuthenticatedSymmetricCipherAbstract
{
    /**
     * Constructor
     * @param \Cryptopp\BlockCipherInterface $cipher block cipher to use. Must be a 128 bits (16 bytes) block size cipher.
     * @throws \Cryptopp\CryptoppException
     */
    public function __construct(BlockCipherInterface $cipher) {}

    /**
     * Sets the digest size. It affects the resulting digest calculation (it is not a simple truncation of the digest).
     * @param int $digestSize one of the following values: 4, 6, 8, 10, 12, 14, or 16
     * @throws \Cryptopp\CryptoppException
     */
    final public function setDigestSize($digestSize) {}

    /**
     * Sets data and AAD size. Must be called before any encryption or decryption operation.
     * Encrypted data length and passed AAD length must match exactly.
     * @param int $dataSize
     * @param int $aadSize
     * @throws \Cryptopp\CryptoppException
     */
    final public function specifyDataSize($dataSize, $aadSize) {}
}
