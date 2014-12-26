<?php

namespace Cryptopp;

interface AuthenticatedSymmetricCipherInterface extends SymmetricTransformationInterface
{
    /**
     * Returns the size of the generated authentication tag (in bytes)
     * @return int
     */
    public function getDigestSize();

    // TODO comment
    public function addEncryptionAdditionalData($data);

    // TODO comment
    public function addDecryptionAdditionalData($data);

    // TODO comment
    public function finalizeEncryption();

    // TODO comment
    public function finalizeDecryption();
}
