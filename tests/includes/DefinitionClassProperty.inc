<?php

class DefinitionClassProperty
{
    /**
     * Property reflector
     * @var \ReflectionProperty
     */
    private $reflector;

    /**
     * Constructor
     * @param string $className class name
     * @param string $propertyName property name
     */
    public function __construct($className, $propertyName)
    {
        if (!is_string($className)) {
            throw new InvalidArgumentException("Class name name must be a string");
        } elseif (!is_string($propertyName)) {
            throw new InvalidArgumentException("Property name name must be a string");
        } elseif (!property_exists($className, $propertyName)) {
            throw new InvalidArgumentException("Property '$classname::\$$methodName' not found");
        }

        $this->reflector = new \ReflectionProperty($className, $propertyName);
    }

    /**
     * Return the property definition
     * @return array
     */
    public function getDefinition()
    {
        $definition = array(
            "name"          => $this->reflector->getName(),
            "defaultValue"  => null,
            "isStatic"      => $this->reflector->isStatic(),
            "visibility"    => null,
        );

        // default value
        $defaultProperties  = $this->reflector->getDeclaringClass()->getDefaultProperties();
        $propertyName       = $this->reflector->getName();

        if (array_key_exists($propertyName, $defaultProperties) && null !== $defaultProperties[$propertyName]) {
            $definition["defaultValue"] = $defaultProperties[$propertyName];
        }

        // visibility
        if ($this->reflector->isPublic()) {
            $definition["visibility"] = "public";
        } elseif ($this->reflector->isProtected()) {
            $definition["visibility"] = "protected";
        } elseif ($this->reflector->isPrivate()) {
            $definition["visibility"] = "private";
        }

        return $definition;
    }
}
