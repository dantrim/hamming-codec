# hamming-codec
Simple encode/decode utilties for single-bit error correcting Hamming codes

# Table of Contents
 1. [Requirements](#requirements)
 2. [Installation](#installation)
     * [Python](#python-installation)
     * [C++](#cpp-installation)
 3. [Python Usage](#python-usage)
     * [Command-line Usage](#from-the-command-line)
     * [Importing the `hamming_codec` Python Module](#importing-as-a-module)
 4. [C++ Usage](#cpp-usage)
     * [Command-line Usage](#from-the-command-line-cpp)
     * [Linking to the Shared Library](#linking-to-the-shared-library)
 5. [Error-correction](#single-bit-error-correction)

[![GitHub Actions Status Badge: CI/CD]][GitHub Actions Status: CI/CD]
[![GitHub Actions Status Badge: cpp_build]][GitHub Actions Status: cpp_build]
[![pre-commit.ci status][pre-commit.ci status badge]][pre-commit.ci status]
[![Code style: black][black badge]](https://github.com/psf/black)

# Requirements

Tested on Python `>=3.6`.

# Installation

## Python Installation

Install using `pip` as follows, after which the command-line utility `hamming` will be available:

```bash
$ git clone https://github.com/dantrim/hamming-codec.git
$ cd hamming-codec
$ python -m venv env
$ source env/bin/activate
$ {env} python -m pip install .
$ {env} hamming -h
Usage: hamming [OPTIONS] COMMAND [ARGS]...

Top-level entrypoint into hamming-codec utilities.

Options:
   -v, --verbose
   -h, --help     Show this message and exit.

Commands:
   decode  Decode the input message that is the specified number of bits in...
   encode  Encode the provided input data word, which is interpreted as
            being...
```

## Cpp Installation
The `hamming-codec` library offers C++ bindings via [pybind11](https://pybind11.readthedocs.io/en/stable/)
which can be built using CMake. As the libary is a Python-first library, it must first be built using
the steps outlined in the [Python Installation](#python-installation) section. This is because the Python
package must be first built and installed in order for [pybind11](https://pybind11.readthedocs.io/en/stable/)
to properly find it and to know which Python runtime to link against.

Follow these steps to ensure a proper build of the C++ bindings to the `hamming-codec` library:
```bash
$ git clone https://github.com/dantrim/hamming-codec.git
$ cd hamming-codec
$ python -m venv env
$ source env/bin/activate
$ {env} python -m pip install .
$ {env} source bash/set_pythonpath.sh
$ {env} deactivate
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Python Usage

### From the Command-line

#### Encoding

After [installation](#python-installation) you can Hamming encode messages of specified length (in number of bits) as follows:

```bash
$ source env/bin/activate
$ {env} hamming encode 0x1234 16
0x2a3a1 21
```

Which shows that the 16-bit message `0x1234` is encoded as a 21-bit word `0x2a3a1`.

#### Decoding

After [installation](#python-installation) you can decode Hamming encoded messages of specified length (in number of bits) as follows:

```bash
$ source env/bin/activate
$ {env} hamming decode 0x2a3a1 21
0x1234 16
```

Which shows that the 21-bit encoded message `0x2a3a1` is decoded back into the 16-bit word `0x1234`.


#### Importing as a Module

Once you have [installed hamming-codec](#python-installation), you can `import` it and perform encoding/decoding as follows:
```python
>>> import hamming_codec
>>> encoded_message = hamming_codec.encode(0x4235, 16)
>>> hex(int(encoded_message,2))
'0x8a3ac'
>>> decoded_message = hamming_codec.decode(encoded_message, 21)
>>> hex(int(decoded_message,2))
'0x4235'
```

## Cpp Usage

### From the Command-line (Cpp)

After following [the steps to build the C++ library](#cpp-installation), two executables are built and available
in your build directory's `bin/` directory. The two executables are called `encode` and `decode`, each of which performs
the same operations as the analogous [python command line utilities](#from-the-command-line):
```bash
$ ./bin/encode 0x4235 16
0x8a3ac 21
$ ./bin/decode 0x8a3ac 21
0x4235 16
```

### Linking to the Shared Library

A shared library `hamming_codec_cpp.{so,dylib}` also gets built after following [the steps to build the C++ library](#cpp-installation),
located under your build directory's `lib/` directory. By linking against this shared library in the
usual manner, you can include the `hamming_codec` library into your own `C++` code as follows:
```c++
#include "hamming_codec.h"
...
uint32_t n_bits = 16;
uint32_t input_message = 0x4235;
std::string input_binary_string = hamming_codec::int2bin(input_message, n_bits);
std::string encoded_message = hamming_codec::encode(input_binary_string, n_bits);
std::cout << "Encoded message: 0x" << std::hex << std::stoul(encoded_message, 0, 2) << std::endl; // prints "Encoded message: 0x8a3ac"
std::string decoded_message = hamming_codec::decode(encoded_message, encoded_message.length());
std::cout << "Decoded message: 0x" << std::hex << std::stoul(decoded_message, 0, 2) << std::endl; // prints "Decoded message: 0x4235"
```

## Single-bit Error Correction

The Hamming encoding algorithm used within `hamming-codec` allows for single-bit error corrections. That
is, during the decoding process, errors in which a single bit has been flipped in the encoded message
can both be detected **and corrected**.

For example, if we flip a single bit
in the encoded message [from a previous section](#encoding) such that the 21-bit word `0x2a3a1` becomes `0x2a1a1`,
you will get the same decoded message as before:

```bash
$ source env/bin/activate
$ {env} hamming encode 0x1234 16
0x2a3a1 21
$ {env} hamming decode 0x2a3a1 21
0x1234 16
$ {env} hamming decode 0x2a1a1 21 # flipped a bit
0x1234 16
```

<!--- LINKS --->
[pre-commit.ci status badge]:https://results.pre-commit.ci/badge/github/dantrim/hamming-codec/main.svg
[pre-commit.ci status]:https://results.pre-commit.ci/latest/github/dantrim/hamming-codec/main
[GitHub Actions Status Badge: CI/CD]:https://github.com/dantrim/hamming-codec/workflows/CI/CD/badge.svg?branch=main
[GitHub Actions Status: CI/CD]:https://github.com/dantrim/hamming-codec/actions?query=workflow%3ACI%2FCD+branch%3Amain
[GitHub Actions Status Badge: cpp_build]:https://github.com/dantrim/hamming-codec/workflows/cpp_build/badge.svg?branch=main
[GitHub Actions Status: cpp_build]:https://github.com/dantrim/hamming-codec/actions?query=workflow%3Acpp_build+branch%3Amain
[black badge]:https://img.shields.io/badge/code%20style-black-000000.svg
