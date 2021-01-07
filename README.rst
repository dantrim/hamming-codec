=============
hamming-codec
=============
Simple encode/decode utilties for single-bit error correcting Hamming codes


|GitHub Actions Status: CI| |GitHub Actions Status: cpp_build|
|pre-commit.ci Status| |Code style: black|

Requirements
============
Tested on Python ``>=3.6``.

Installation
============

Install using `pip` as follows, after which the command-line utility `hamming` will be available:

.. code-block:: bash

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

Hamming Encoding and Decoding
=============================

Encoding
--------

After installation you can Hamming encode messages of specified length (in number of bits) as follows:

.. code-block:: bash

   $ hamming encode 0x1234 16
   0x2a3a1 21

Which shows that the 16-bit message ``0x1234`` is encoded as a 21-bit word ``0x2a3a1``.

Decoding
--------
After installation you can decode Hamming encoded messages of specified length (in number of bits) as follows:

.. code-block:: bash

   $ hamming decode 0x2a3a1 21
   0x1234 16

Which shows that the 21-bit encoded message `0x2a3a1` is decoded back into the 16-bit word `0x1234`.

Single-bit Error Detection
--------------------------
Single-bit errors are detected and corrected in the decoding process. For example, if we flip a single bit
in the encoded message in the previous section such that the 21-bit word `0x2a3a1` becomes `0x2a1a1`,
you will get the same decoded message as before:

.. code-block:: bash
   $ hamming decode 0x2a1a1 21
   0x1234 16



.. |GitHub Actions Status: CI| image:: https://github.com/dantrim/hamming-codec/workflows/CI/CD/badge.svg?branch=main
   :target: https://github.com/dantrim/hamming-codec/actions?query=workflow%3ACI%2FCD+branch%3Amain
.. |GitHub Actions Status: cpp_build| image:: https://github.com/dantrim/hamming-codec/workflows/cpp_build/badge.svg?branch=main
   :target: https://github.com/dantrim/hamming-codec/actions?query=workflow%3Acpp_build+branch%3Amain 
.. |Code style: black| image:: https://img.shields.io/badge/code%20style-black-000000.svg
   :target: https://github.com/psf/black
.. |pre-commit.ci status| image:: https://results.pre-commit.ci/badge/github/dantrim/hamming-codec/main.svg
   :target: https://results.pre-commit.ci/latest/github/dantrim/hamming-codec/main
   :alt: pre-commit.ci status
