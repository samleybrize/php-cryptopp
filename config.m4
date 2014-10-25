CFLAGS="$CFLAGS -Wall"
dnl CXXFLAGS+=" -std=c++0x "

PHP_ARG_ENABLE(cryptopp_test,
    [Whether to enable the "cryptopp_test" extension],
    [ --enable-cryptopp-test Enable "cryptopp_test" extension support])

if test $PHP_CRYPTOPP_TEST != "no"; then
    dnl search for cryptopp headers
    for i in $PHP_CRYPTOPP_TEST /usr/local /usr; do
        test -f $i/include/cryptopp/aes.h && CRYPTOPP_DIR=$i/include/cryptopp && break
    done

    if test -z "$CRYPTOPP_DIR"; then
        AC_MSG_ERROR(aes.h not found. Please reinstall crypto++.)
    fi

    dnl retrieve cpp file list
    test ! -x "./config.sh" && AC_MSG_ERROR(config.sh must be executable)
    SRC_FILE_LIST=$(CRYPTOPP_DIR=$CRYPTOPP_DIR ./config.sh)

    if test ! $? -eq 0; then
        AC_MSG_ERROR($SRC_FILE_LIST)
    fi


    PHP_ADD_INCLUDE($CRYPTOPP_DIR)
    PHP_REQUIRE_CXX()
    PHP_SUBST(CRYPTOPP_TEST_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, CRYPTOPP_TEST_SHARED_LIBADD)
    PHP_ADD_LIBRARY(cryptopp, 1, CRYPTOPP_TEST_SHARED_LIBADD)
    PHP_NEW_EXTENSION(cryptopp_test, $SRC_FILE_LIST, $ext_shared)
fi

