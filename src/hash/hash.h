#ifndef HASH_HASH_H
#define HASH_HASH_H

#define hash(hash, digestSize, digestName, msg, msdSize) {     \
    php_printf(digestName);                                    \
    php_printf(" digest size : %ld\n", digestSize);            \
    byte digest[digestSize];                                   \
    hash.CalculateDigest(digest, msg, msgSize);                \
    outputByteArray(digest, digestSize);                       \
}

#endif

