<?php

class DefinitionFunctionParameter
{
    /**
     * Parameter reflector
     * @var \ReflectionParameter
     */
    private $reflector;

    /**
     * Constructor
     * @param \ReflectionParameter $reflector parameter reflector
     */
    public function __construct(\ReflectionParameter $reflector)
    {
        $this->reflector = $reflector;
    }

    /**
     * Return the function parameter definition
     * @return array
     */
    public function getDefinition()
    {
        $definition = array(
            "name"                  => $this->reflector->getName(),
            "isPassedByReference"   => $this->reflector->isPassedByReference(),
            "isArray"               => $this->reflector->isArray(),
            "class"                 => $this->reflector->getClass() ? $this->reflector->getClass()->getName() : null,
            "isOptional"            => $this->reflector->isOptional(),
            "isVariadic"            => version_compare(phpversion(), "5.6.0", ">=") && $this->reflector->isVariadic(),
            "defaultValue"          => null,
        );

        // default value
        if ($this->reflector->isDefaultValueAvailable()) {
            if (version_compare(phpversion(), "5.4.6", ">=") && $this->reflector->isDefaultValueConstant()) {
                // default value as constant
                $definition["defaultValue"] = $this->reflector->getDefaultValueConstantName();
            } else {
                $definition["defaultValue"] = $this->reflector->getDefaultValue();
            }
        }

        return $definition;
    }
}
