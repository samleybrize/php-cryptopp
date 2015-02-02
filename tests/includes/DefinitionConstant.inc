<?php

class DefinitionConstant
{
    /**
     * Constant name
     * @var string
     */
    private $name;

    /**
     * Constant value
     * @var scalar
     */
    private $value;

    /**
     * Constructor
     * @param string $name constant name
     * @param scalar $value constant value
     */
    public function __construct($name, $value)
    {
        if (!is_string($name)) {
            throw new InvalidArgumentException("Constant name must be a string");
        } elseif (!is_scalar($value)) {
            throw new InvalidArgumentException("Constant value must be scalar");
        }

        $this->name     = $name;
        $this->value    = $value;
    }

    /**
     * Return the constant definition
     * @return array
     */
    public function getDefinition()
    {
        $definition = array(
            "name"      => $this->name,
            "value"     => $this->value,
        );

        return $definition;
    }
}
