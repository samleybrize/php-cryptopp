/*
 * This file is part of php-cryptopp.
 *
 * (c) Stephen Berquet <stephen.berquet@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_AUTHENTICATED_SYMMETRIC_CIPHER_PRESPECIFIED_LENGTHS_H
#define PHP_AUTHENTICATED_SYMMETRIC_CIPHER_PRESPECIFIED_LENGTHS_H

/* {{{ TODO */
class AuthenticatedSymmetricCipherPrespecifiedLengths
{
public:
    virtual ~AuthenticatedSymmetricCipherPrespecifiedLengths(){}
    virtual void ProcessData(byte *outString, const byte *inString, size_t length) =0;
    virtual void Update(const byte *input, size_t length) =0;
    virtual void Final(byte *digest) =0;
};
/* }}} */

#endif /* PHP_AUTHENTICATED_SYMMETRIC_CIPHER_PRESPECIFIED_LENGTHS_H */
