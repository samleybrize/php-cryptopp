<?php

namespace Cryptopp;

// TODO comment
interface BlockCipherInterface extends SymmetricCipherInterface
{
    // TODO comment
    public function encrypt($data) {}

    // TODO comment
    public function decrypt($data) {}

    // TODO comment
    public function encryptBlock($block) {}

    // TODO comment
    public function decryptBlock($block) {}
}
