#ifndef PHP_SYMMETRIC_TRANSFORMATION_USER_INTERFACE_H
#define PHP_SYMMETRIC_TRANSFORMATION_USER_INTERFACE_H

#include "src/php_cryptopp.h"

class SymmetricTransformationUserInterface
{
public:
    virtual ~SymmetricTransformationUserInterface(){}
    virtual bool IsValidIvLength(size_t n) =0;
    virtual void Restart() =0;
};

#endif /* PHP_SYMMETRIC_TRANSFORMATION_USER_INTERFACE_H */
