# PHP Crypto++ bindings

Provides an object-oriented API to use a wide variety of cryptographic algorithms based on the Crypto++ library. The API is not strictly identical to the original Crypto++ API and has been simplified to be usable by anyone that has not any knowledge in cryptography.

[![Build Status](https://travis-ci.org/samleybrize/php-cryptopp.svg?branch=master)](https://travis-ci.org/samleybrize/php-cryptopp)

## Requirements

- PHP 5.3+
- Crypto++ library
- Python 2.7+ (make)

The Crypto++ library can be installed with one of the following commands, depending on your distribution:
- Ubuntu: `sudo apt-get install libcrypto++-dev`
- Debian: `sudo apt-get install libcrypto++-dev`
- ArchLinux: `sudo pacman -S crypto++`
- CentOS/RedHat: `yum install cryptopp-devel`

## Installation

```sh
phpize
./configure
make
sudo make install
```

Now you can add the following line to your `php.ini` file:

```ini
    extension=cryptopp.so
```

## Documentation

Full documentation is not finalized yet. You can see the current version in the [wiki](https://github.com/samleybrize/php-cryptopp/wiki).

See the PHP stubs available in the [php-stubs](php-stubs) directory, which may be also used in your IDE for code completion.

## Status

Available
- Symmetric encryption
- Hash functions
- MAC algorithms

Comming
- Asymmetric cryptography
- Key derivation functions

## Author

This project is authored and maintained by Stephen Berquet.

## License

Licensed under the MIT License - see the [LICENSE](LICENSE) file for details
