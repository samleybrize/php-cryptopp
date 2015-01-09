<?php

namespace Cryptopp;

class RandomByteGenerator implements RandomByteGeneratorInterface
{
    /**
     * Constructor
     */
    public function __construct() {}

    /**
     * {@inheritdoc}
     */
    final public function generate($size) {}

    /**
     * Disables object serialization
     */
    final public function __sleep() {}
    
    /**
     * Disables object serialization
     */
    final public function __wakeup() {}
}
