<?php

namespace Cryptopp;

interface SymmetricTransformationInterface extends SymmetricCipherInterface
{
    // TODO comment
    public function setIv($iv);

    // TODO comment
    public function encrypt($data);

    // TODO comment
    public function decrypt($data);

    // TODO comment
    public function restart();
}
