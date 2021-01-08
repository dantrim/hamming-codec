# hamming-codec
Simple encode/decode utilties for single-bit error correcting Hamming codes

[![GitHub Actions Status Badge: CI/CD]][GitHub Actions Status: CI/CD]
[![GitHub Actions Status Badge: cpp_build]][GitHub Actions Status: cpp_build]
[![pre-commit.ci status][pre-commit.ci status badge]][pre-commit.ci status]
[![Code style: black][black badge]](https://github.com/psf/black)

Requirements
============
Tested on Python `>=3.6`.

Installation
============

Install using `pip` as follows, after which the command-line utility `hamming` will be available:

```bash
$ git clone https://github.com/dantrim/hamming-codec.git
$ cd hamming-codec
$ python -m pip install .
$ hamming -h
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

## Hamming Encoding and Decoding

### Encoding

After installation you can Hamming encode messages of specified length (in number of bits) as follows:

```bash
$ hamming encode 0x1234 16
0x2a3a1 21
```

Which shows that the 16-bit message `0x1234` is encoded as a 21-bit word `0x2a3a1`.

### Decoding

After installation you can decode Hamming encoded messages of specified length (in number of bits) as follows:

```bash
$ hamming decode 0x2a3a1 21
0x1234 16
```

Which shows that the 21-bit encoded message `0x2a3a1` is decoded back into the 16-bit word `0x1234`.

### Single-bit Error Detection

Single-bit errors are detected and corrected in the decoding process. For example, if we flip a single bit
in the encoded message in the previous section such that the 21-bit word `0x2a3a1` becomes `0x2a1a1`,
you will get the same decoded message as before:

```bash
$ hamming decode 0x2a1a1 21
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
