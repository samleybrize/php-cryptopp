#include "../../../php_cryptopp.h"
#include "stream_cipher_proxy.h"

StreamCipherProxy::Base::Base(zval *streamCipherObject)
{
    // TODO accept a zval (instance of StreamCipherInterface)
}

unsigned int StreamCipherProxy::Base::MandatoryBlockSize() const
{
    // TODO
    php_printf("MandatoryBlockSize\n");
    return 16;
}

unsigned int StreamCipherProxy::Base::OptimalBlockSize() const
{
    // TODO
    php_printf("OptimalBlockSize\n");
    return 16;
}

void StreamCipherProxy::Encryption::ProcessData(byte *outString, const byte *inString, size_t length)
{
    // TODO
    php_printf("ProcessData (enc)\n");
    memcpy(outString, inString, length);
}

void StreamCipherProxy::Decryption::ProcessData(byte *outString, const byte *inString, size_t length)
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
