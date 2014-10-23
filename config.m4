CFLAGS="$CFLAGS -Wall"
dnl CXXFLAGS+=" -std=c++0x "

PHP_ARG_ENABLE(cryptopp_test,
    [Whether to enable the "cryptopp_test" extension],
    [ --enable-cryptopp-test Enable "cryptopp_test" extension support])

if test $PHP_CRYPTOPP_TEST != "no"; then
    for i in $PHP_CRYPTOPP_TEST /usr/local /usr; do
        test -f $i/include/cryptopp/aes.h && CRYPTOPP_DIR=$i/include/cryptopp && break
    done

    if test -z "$CRYPTOPP_DIR"; then
        AC_MSG_ERROR(aes.h not found. Please reinstall crypto++.)
    fi

    SRC_FILE_LIST=$(find src -name *.cpp)
    PHP_ADD_INCLUDE($CRYPTOPP_DIR)
    PHP_REQUIRE_CXX()
    PHP_SUBST(CRYPTOPP_TEST_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, CRYPTOPP_TEST_SHARED_LIBADD)
    PHP_ADD_LIBRARY(cryptopp, 1, CRYPTOPP_TEST_SHARED_LIBADD)
    PHP_NEW_EXTENSION(cryptopp_test, $SRC_FILE_LIST, $ext_shared)
fi

