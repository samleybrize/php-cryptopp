<?php

namespace Cryptopp;

interface AuthenticatedSymmetricCipherInterface extends SymmetricTransformationInterface
{
    /**
     * Returns the size of the generated authentication tag (in bytes)
     * @return int
     */
    public function getDigestSize();

    /**
     * Adds additional authenticated data (AAD) to encryption MAC calculation. Such data must be added before any encryption has been performed.
     * @param string $data data to add
     * @throws \Cryptopp\CryptoppException
     */
    public function addEncryptionAdditionalData($data);

    /**
     * Adds additional authenticated data (AAD) to decryption MAC calculation. Such data must be added before any decryption has been performed.
     * @param string $data data to add
     * @throws \Cryptopp\CryptoppException
     */
    public function addDecryptionAdditionalData($data);

    /**
     * Finalize encryption and return the MAC tag
     * @return string MAC tag
     * @throws \Cryptopp\CryptoppException
     */
    public function finalizeEncryption();

    /**
     * Finalize decryption and return the MAC tag
     * @return string MAC tag
     * @throws \Cryptopp\CryptoppException
     */
    public function finalizeDecryption();
}
