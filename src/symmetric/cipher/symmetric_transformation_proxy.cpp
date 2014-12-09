#include "../../php_cryptopp.h"
#include "symmetric_transformation_proxy.h"

SymmetricTransformationProxy::Base::Base(zval *streamCipherObject)
{
    // TODO accept a zval (instance of StreamCipherInterface)
}

unsigned int SymmetricTransformationProxy::Base::MandatoryBlockSize() const
{
    // TODO
    php_printf("MandatoryBlockSize\n");
    return 16;
}

unsigned int SymmetricTransformationProxy::Base::OptimalBlockSize() const
{
    // TODO
    php_printf("OptimalBlockSize\n");
    return 16;
}

void SymmetricTransformationProxy::Encryption::ProcessData(byte *outString, const byte *inString, size_t length)
{
    // TODO
    php_printf("ProcessData (enc)\n");
    memcpy(outString, inString, length);
}

void SymmetricTransformationProxy::Decryption::ProcessData(byte *outString, const byte *inString, size_t length)
{
    // TODO
    php_printf("ProcessData (dec)\n");
    memcpy(outString, inString, length);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */
