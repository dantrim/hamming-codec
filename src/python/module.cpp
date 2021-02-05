#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include <iostream>

#include "hamming_codec.h"
#include "utils.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

PYBIND11_MODULE(_hamming_codec, m) {
    m.doc() = "Simple encode/decode utilities for single-bit error correcting Hamming codes";

    m.def("n_parity_bits", &hamming_codec::n_parity_bits_required,
            R"pbdoc(
            Compute the number of parity bits required for encoding an input message of a specific length.

            Parameters
            ----------
            n_bits: int
                The number of bits in the input message to be encoded.

            Returns
            -------
            int:
                The number of redundant/parity bits required to encode the message.
            )pbdoc",
            py::arg("n_bits")
            );
    m.def("compute_parity_bit_positions", &hamming_codec::compute_parity_bit_positions,
            R"pbdoc(
            Computes the bit positions (indices) of the redundant/parity bits in an encoded
            message that contains the specified number of redundant/parity bits.

            The redundant/parity bits are located at positions whose indices are
            exact powers of 2, so that the first parity bit is at (counting from zero)
            bit position 2^0=1, the second parity bit is at bit position 2^1=2,
            the third parity bit is at bit position 2^2=4, and so on.

            This method returns an array containing these bit positions, whose length
            is equal to the number of redundant/parity bits specified.

            Parameters
            ----------
            n_parity_bits: int
                The number of redundant/parity bits in the encoded message.

            Returns
            -------
            list[int]:
                A list of integers whose length is equal to the number of redundant/parity bits
                in the encoded message and whose values are the positions of the corresponding
                redundant/parity bits in the encoded message.
                
            
            )pbdoc",
            py::arg("n_parity_bits")
            );
    m.def("compute_parity_bits", &hamming_codec::compute_parity_bits);
    m.def("n_parity_bits_required", &hamming_codec::n_parity_bits_required);
    m.def("int2bin", &hamming_codec::utils::int2bin);
    m.def("encode", &hamming_codec::encode, "Encode an input value");
    m.def("decode", &hamming_codec::decode, "Decode an input value");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
