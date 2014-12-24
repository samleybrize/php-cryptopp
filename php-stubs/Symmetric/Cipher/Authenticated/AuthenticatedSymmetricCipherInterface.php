<?php

namespace Cryptopp;

// TODO comment
interface AuthenticatedSymmetricCipherInterface extends SymmetricTransformationInterface
{
    // TODO comment
    public function getDigestSize() {}

    // TODO comment
    public function addEncryptionAdditionalData($data) {}

    // TODO comment
    public function addDecryptionAdditionalData($data) {}

    // TODO comment
    public function finalizeEncryption() {}

    // TODO comment
    public function finalizeDecryption() {}
}
