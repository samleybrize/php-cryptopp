<?php

require_once __DIR__ . "/DefinitionFunctionParameter.inc";

class DefinitionClassMethod
{
    /**
     * Method reflector
     * @var \ReflectionMethod
     */
    private $reflector;

    /**
     * Constructor
     * @param string $className class name
     * @param string $methodName method name
     */
    public function __construct($className, $methodName)
    {
        if (!is_string($className)) {
            throw new InvalidArgumentException("Class name name must be a string");
        } elseif (!is_string($methodName)) {
            throw new InvalidArgumentException("Method name must be a string");
        } elseif (!method_exists($className, $methodName)) {
            throw new InvalidArgumentException("Method '$classname::$methodName' not found");
        }

        $this->reflector = new \ReflectionMethod($className, $methodName);
    }

    /**
     * Return the method definition
     * @return array
     */
    public function getDefinition()
    {
        $definition = array(
            "name"              => $this->reflector->getName(),
            "isStatic"          => $this->reflector->isStatic(),
            "isAbstract"        => $this->reflector->isAbstract(),
            "isFinal"           => $this->reflector->isFinal(),
            "returnsReference"  => $this->reflector->returnsReference(),
            "visibility"        => null,
            "args"              => array(),
        );

        // visibility
        if ($this->reflector->isPublic()) {
            $definition["visibility"] = "public";
        } elseif ($this->reflector->isProtected()) {
            $definition["visibility"] = "protected";
        } elseif ($this->reflector->isPrivate()) {
            $definition["visibility"] = "private";
        }

        // args
        $argList = $this->reflector->getParameters();

        foreach ($argList as $arg) {
            $generator              = new DefinitionFunctionParameter($arg);
            $definition["args"][]   = $generator->getDefinition();
        }

        return $definition;
    }
}
