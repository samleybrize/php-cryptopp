CFLAGS="$CFLAGS -Wall"
dnl CXXFLAGS+=" -std=c++0x "

PHP_ARG_ENABLE(cryptopp,
    [Whether to enable the "crypto++" extension],
    [ --enable-cryptopp Enable "crypto++" extension support])

if test $PHP_CRYPTOPP != "no"; then
    dnl search for cryptopp headers
    for i in $PHP_CRYPTOPP /usr/local /usr; do
        test -f $i/include/cryptopp/aes.h && CRYPTOPP_DIR=$i/include/cryptopp && break
    done

    if test -z "$CRYPTOPP_DIR"; then
        AC_MSG_ERROR(aes.h not found. Please reinstall crypto++.)
    fi

    dnl retrieve cpp file list
    SRC_FILE_LIST=$(/usr/bin/env python ./config.py $CRYPTOPP_DIR)

    if test ! $? -eq 0; then
        AC_MSG_ERROR($SRC_FILE_LIST)
    fi


    PHP_ADD_INCLUDE($CRYPTOPP_DIR)
    PHP_REQUIRE_CXX()
    PHP_SUBST(CRYPTOPP_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, CRYPTOPP_SHARED_LIBADD)
    PHP_ADD_LIBRARY(cryptopp, 1, CRYPTOPP_SHARED_LIBADD)
    PHP_NEW_EXTENSION(cryptopp, $SRC_FILE_LIST, $ext_shared)
fi
