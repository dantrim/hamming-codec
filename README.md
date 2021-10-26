# hamming-codec
Simple encode/decode utilties for single-bit error correcting Hamming codes

[![GitHub Actions Status Badge: CI/CD]][GitHub Actions Status: CI/CD]
[![GitHub Actions Status Badge: cpp_build]][GitHub Actions Status: cpp_build]
![centos7_build](https://github.com/dantrim/hamming-codec/workflows/centos7_build/badge.svg)
[![pre-commit.ci status][pre-commit.ci status badge]][pre-commit.ci status]
[![Code style: black][black badge]](https://github.com/psf/black)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

# Table of Contents
 1. [Requirements](#requirements)
 2. [Installation](#installation)
     * [Python](#python-installation)
     * [C++](#c-installation)
 3. [Python Usage](#python-usage)
     * [Command-line Usage](#from-the-command-line)
     * [Importing the `hamming_codec` Python Module](#importing-as-a-module)
 4. [C++ Usage](#c-usage)
     * [Including in your Project](#including-in-your-project)
     * [Examples](#c-examples)
 5. [Error-correction](#single-bit-error-correction)
 6. [References](#references)

# Requirements

Tested on Python `>=3.6` and requires C++ compilers supporting C++17 features.


# Installation

## Python Installation

Install using `pip` as follows:

```bash
$ python -m pip install hamming-codec
```

## C++ Installation

If you wish to use the `hamming-codec` encoding/decoding from within C++,
one needs to include the single header file
[src/cpp/hamming_codec.h](src/cpp/hamming_codec.h) in their project.
There are a few ways to do this:

  1. Copy the file [hamming_codec.h](src/cpp/hamming_codec.h) into your project's include path.
  2. Add `hamming-codec` as a sub-module (or equivalent) and use CMake to expose the `HAMMING_CODEC_INCLUDE_DIRS`
        variable. See [src/cpp/examples/CMakeLists.txt](src/cpp/examples/CMakeLists.txt) for an example.

Following either approach, you should be able to add `#include "hamming_codec.h"` to your
source code.

## Python Usage

### From the Command-line

After [following the Python installation](#python-installation), a commandline utility
`hamming` will be available to you, which can be used for simple encoding and decoding
of data words. Use `hamming --help` to show the full set of options and commands.

#### Encoding

You can use the `hamming` commandline utility to encode messages of specified length
(in number of bits) as follows:

```bash
$ hamming encode 0x1234 16
0x2a3a1 21
```

Which shows that the 16-bit message `0x1234` is encoded as a 21-bit word `0x2a3a1`.

#### Decoding

You can use the `hamming` commandline utility to decode messages of specified length (in number of bits) as follows:

```bash
$ hamming decode 0x2a3a1 21
0x1234 16
```
Which shows that the 21-bit encoded message `0x2a3a1` is decoded back into the 16-bit word `0x1234`.


#### Importing as a Module

Once you have [installed hamming-codec](#python-installation), you can import the `hamming_codec`
module and perform encoding/decoding like so:

```python
>>> import hamming_codec
>>> encoded_message = hamming_codec.encode(0x4235, 16)
>>> print(encoded_message)
010001010001110101100
>>> hex(int(encoded_message,2))
'0x8a3ac'
>>> decoded_message = hamming_codec.decode(int(encoded_message,2), len(encoded_message))
>>> print(decoded_message)
0100001000110101
>>> hex(int(decoded_message,2))
'0x4235'
```

## C++ Usage

### Including in your Project

Following the [steps to add `hamming-codec` to your include path](#c-installation),
you can encode/decode messages as follows:

```c++
#include "hamming_codec.h"
...
uint32_t n_bits = 16;
uint32_t input_message = 0x4235;
std::string encoded_message = hamming_codec::encode(input_message, n_bits);
std::cout << "Encoded message: 0x" << std::hex << std::stoul(encoded_message, 0, 2) << std::endl; // prints "Encoded message: 0x8a3ac"
std::string decoded_message = hamming_codec::decode(std::stoul(encoded_message, 0, 2), encoded_message.length());
std::cout << "Decoded message: 0x" << std::hex << std::stoul(decoded_message, 0, 2) << std::endl; // prints "Decoded message: 0x4235"
```

### C++ Examples
After following the [steps to build the C++ library](#c-installation), you 
can run the C++ examples. For example,
```bash
$ ./build/bin/example_encode 0x4235 16
0x8a3ac 21
```

## Single-bit Error Correction

The Hamming encoding algorithm used within `hamming-codec` allows for single-bit error corrections. That
is, during the decoding process, errors in which a single bit has been flipped in the encoded message
can both be detected **and corrected**.

For example, if we flip a single bit
in the encoded message [from a previous section](#encoding) such that the 21-bit word `0x2a3a1` becomes `0x2a1a1`,
you will get the same decoded message as before:

```bash
$ hamming encode 0x1234 16
0x2a3a1 21
$ hamming decode 0x2a3a1 21
0x1234 16
$ hamming decode 0x2a1a1 21 # flipped a bit
0x1234 16 # ...but decodes the same as before!
```

## References
Further information about the Hamming encoding algorithm employed within `hamming-codec` can be found in the following resources:
  * [3Blue1Brown's "Hamming codes and error correction"](https://www.youtube.com/watch?v=X8jsijhllIA)
  * [Ben Eater's "What is error correction? Hamming codes in hardware"](https://www.youtube.com/watch?v=h0jloehRKas)
  * [tutorialspoint's "Error Correcting Codes - Hamming codes"](https://www.tutorialspoint.com/error-correcting-codes-hamming-codes)
  * [Wikipedia entry for "Hamming Code"](https://en.wikipedia.org/wiki/Hamming_code)
  * [Wikipedia entry for "Linear error-correcting codes"](https://en.wikipedia.org/wiki/Linear_code)

<!--- LINKS --->
[pre-commit.ci status badge]:https://results.pre-commit.ci/badge/github/dantrim/hamming-codec/main.svg
[pre-commit.ci status]:https://results.pre-commit.ci/latest/github/dantrim/hamming-codec/main
[GitHub Actions Status Badge: CI/CD]:https://github.com/dantrim/hamming-codec/workflows/CI/CD/badge.svg?branch=main
[GitHub Actions Status: CI/CD]:https://github.com/dantrim/hamming-codec/actions?query=workflow%3ACI%2FCD+branch%3Amain
[GitHub Actions Status Badge: cpp_build]:https://github.com/dantrim/hamming-codec/workflows/cpp_build/badge.svg?branch=main
[GitHub Actions Status: cpp_build]:https://github.com/dantrim/hamming-codec/actions?query=workflow%3Acpp_build+branch%3Amain
[black badge]:https://img.shields.io/badge/code%20style-black-000000.svg
