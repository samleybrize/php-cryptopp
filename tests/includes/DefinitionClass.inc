<?php

require_once __DIR__ . "/DefinitionClassProperty.inc";
require_once __DIR__ . "/DefinitionClassMethod.inc";

class DefinitionClass
{
    /**
     * Class reflector
     * @var \ReflectionClass
     */
    private $reflector;

    /**
     * Constructor
     * @param string $classname class name
     */
    public function __construct($classname)
    {
        if (!class_exists($classname)) {
            throw new InvalidArgumentException("Class '$classname' not found");
        }

        $this->reflector = new \ReflectionClass($classname);
    }

    /**
     * Return the class definition
     * @return array
     */
    public function getDefinition()
    {
        $definition = array(
            "name"          => $this->reflector->getShortName(),
            "isTrait"       => $this->reflector->isTrait(),
            "isInterface"   => $this->reflector->isInterface(),
            "isAbstract"    => $this->reflector->isAbstract(),
            "isFinal"       => $this->reflector->isFinal(),
            "extends"       => $this->reflector->getParentClass() ? $this->reflector->getParentClass()->getShortName() : null,
            "implements"    => $this->reflector->getInterfaceNames(),
            "traits"        => array(),
            "constants"     => $this->reflector->getConstants(),
            "properties"    => array(),
            "methods"       => array(),
        );

        // traits
        if (version_compare(phpversion(), "5.4.0", ">=")) {
            $definition["traits"] = $this->reflector->getTraitNames();
        }

        // properties
        $propertyList = $this->reflector->getProperties(\ReflectionProperty::IS_PUBLIC | \ReflectionProperty::IS_PROTECTED);

        foreach ($propertyList as $property) {
            // exclude properties declared by parent classes
            if ($property->getDeclaringClass()->getName() != $this->reflector->getName()) {
                continue;
            } elseif ($this->isPropertyFromTrait($property)) {
                continue;
            }

            // definition
            $generator                                      = new DefinitionClassProperty($this->reflector->getName(), $property->getName());
            $definition["properties"][$property->getName()] = $generator->getDefinition();
        }

        ksort($definition["properties"]);

        // methods
        $methodList = $this->reflector->getMethods(\ReflectionProperty::IS_PUBLIC | \ReflectionProperty::IS_PROTECTED);

        foreach ($methodList as $method) {
            // exclude methods declared by parent classes
            if ($method->getDeclaringClass()->getName() != $this->reflector->getName()) {
                continue;
            } elseif ($this->isMethodFromTrait($method)) {
                continue;
            }

            // definition
            $generator                                  = new DefinitionClassMethod($this->reflector->getName(), $method->getName());
            $definition["methods"][$method->getName()]  = $generator->getDefinition();
        }

        ksort($definition["methods"]);

        return $definition;
    }

    /**
     * Indicates if a property is declared in a trait
     * @param \ReflectionProperty $property
     * @return boolean
     */
    protected function isPropertyFromTrait(\ReflectionProperty $property)
    {
        $propertyName   = $property->getName();
        $class          = $property->getDeclaringClass();
        $traitList      = $class->getTraits();

        if ($traitList) {
            foreach ($traitList as $trait) {
                try {
                    $trait->getProperty($propertyName);
                    return true;
                } catch (\ReflectionException $e) {
                }
            }
        }

        return false;
    }

    /**
     * Indicates if a method is declared in a trait
     * @param \ReflectionMethod $method
     * @return boolean
     */
    protected function isMethodFromTrait(\ReflectionMethod $method)
    {
        $methodName     = $method->getName();
        $class          = $method->getDeclaringClass();
        $traitList      = $class->getTraits();

        if ($traitList) {
            $generatorMethod    = new GeneratorClassMethod($class->getName(), $methodName);
            $signatureMethod    = $generatorMethod->generate();

            foreach ($traitList as $trait) {
                try {
                    $traitMethod    = $trait->getMethod($methodName);
                    $generatorTrait = new GeneratorClassMethod($trait->getName(), $methodName);
                    $signatureTrait = $generatorTrait->generate();

                    if ($signatureMethod === $signatureTrait) {
                        return true;
                    }
                } catch (\ReflectionException $e) {
                }
            }
        }

        return false;
    }
}
