/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

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
