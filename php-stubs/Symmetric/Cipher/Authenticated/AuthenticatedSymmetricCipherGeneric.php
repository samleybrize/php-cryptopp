<?php

namespace Cryptopp;

class AuthenticatedSymmetricCipherGeneric extends AuthenticatedSymmetricCipherAbstract
{
    /**
     * Constructor
     * @param \Cryptopp\SymmetricTransformationInterface $cipher symmetric cipher to use. Instances of \Cryptopp\AuthenticatedSymmetricCipherInterface are not allowed.
     * @param \Cryptopp\MacInterface $mac message authentication code algorithm to use
     * @throws \Cryptopp\CryptoppException
     */
    public function __construct(SymmetricTransformationInterface $cipher, MacInterface $mac) {}

    /**
     * Returns the symmetric cipher used
     * @return \Cryptopp\SymmetricTransformationInterface
     */
    final public function getCipher() {}

    /**
     * Returns the message authentication code algorithm used
     * @return \Cryptopp\MacInterface
     */
    final public function getMac() {}

    /**
     * Sets the cipher key
     * @param string $key
     * @throws \Cryptopp\CryptoppException
     */
    final public function setKey($key);

    /**
     * Sets the MAC key
     * @param string $key
     * @throws \Cryptopp\CryptoppException
     */
    final public function setMacKey($key) {}
}
