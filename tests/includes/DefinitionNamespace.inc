<?php

require_once __DIR__ . "/DefinitionClass.inc";
require_once __DIR__ . "/DefinitionConstant.inc";

class DefinitionNamespace
{
    /**
     * Namespace name
     * @var string
     */
    private $name;

    /**
     * Constructor
     * @param string $name namespace name
     */
    public function __construct($name)
    {
        $this->name = (string) "$name\\";
    }

    /**
     * Return the namespace definition
     * @return array
     */
    public function getDefinition()
    {
        $definition = array();

        // classes
        $classList = get_declared_classes();

        foreach ($classList as $class) {
            // check namespace
            if (0 !== strpos($class, $this->name)) {
                continue;
            }

            // add definition
            $generator                      = new DefinitionClass($class);
            $definition["class"][$class]    = $generator->getDefinition();
        }

        // constants
        $constants = get_defined_constants();

        foreach ($constants as $name => $value) {
            // check namespace
            if (0 !== strpos($name, $this->name)) {
                continue;
            }

            $shortName                      = substr($name, strrpos($name, "\\") + 1);
            $generator                      = new DefinitionConstant($shortName, $value);
            $definition["constant"][$name]  = $generator->getDefinition();
        }

        ksort($definition);
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
