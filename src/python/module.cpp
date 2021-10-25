#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include <iostream>

#include "hamming_codec.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

PYBIND11_MODULE(_hamming_codec, m) {
    m.doc() = "Simple encode/decode utilities for single-bit error correcting Hamming codes";

    m.def("n_parity_bits", &hamming_codec::internal::n_parity_bits_required,
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
    m.def("compute_parity_bit_positions", &hamming_codec::internal::compute_parity_bit_positions,
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
    m.def("compute_parity_bits", &hamming_codec::internal::compute_parity_bits,
            R"pbdoc(
            Computes the values for the redundant/parity bits based on the input
            binary string which has the positiosn of the redundant/parity
            bit positions specified.

            The returned list of parity bits will always be of the same length
            as the input binary string.

            In the encoding stage the returned array of parity bits will contain
            the values that the redundant/parity bits should take in the encoded
            message. In the decoding stage, the returned array of parity bits will
            be non-zero if single-bit error is detected and the returned array,
            interpreted as a binary string, will indicate the bit position
            at which the bit flip has occurred.

            Parameters
            ----------
            binary_string: str
                Binary string whose length (in number of bits) is equal to the length of the
                encoded message (number of redundant/parity bits + number of bits in the
                message to encode).

            positions: list[int]
                List containing the indices (positions) of the redundant/parity bits
                in the input binary string.

            inclusive: bool
                Whether or not to include redundant/parity bits in the computation of parity
                (False when encoding, True when decoding and using error-correction).

            Returns
            -------
            list[int]
                An array with the parity bit values. In the encoding stage, it provides
                the values that the redundant bits should take and in the decoding
                stage, if non-zero, it indicates the position of a single-bit error.
            )pbdoc",
            py::arg("binary_string"), py::arg("positions"), py::arg("inclusive")
            );

    m.def("n_parity_bits_required", &hamming_codec::internal::n_parity_bits_required,
            R"pbdoc(
            Computes the number of redundant/parity bits required to encode
            a message of specified length (in number of bits).

            Parameters
            ----------
            n_bits: int
                The number of bits in the input message (prior to encoding).

            Returns
            -------
            int
                The number of redundant/parity bits required.
            )pbdoc",
            py::arg("n_bits")
    );
    m.def("int2bin", &hamming_codec::utils::int2bin,
            R"pbdoc(
            Convert an integer into its binary representation.

            Parameters
            ----------
            data: int
                The integer number to represent in binary.
            n_bits: int
                The size in number of bits to interpret the input data as.

            Returns
            -------
            str
                The binary string representation of the input data.
            )pbdoc",
            py::arg("data"), py::arg("n_bits")
    );

    py::enum_<hamming_codec::ParityLocation>(m, "ParityLocation")
        .value("DEFAULT", hamming_codec::ParityLocation::DEFAULT)
        .value("MSB", hamming_codec::ParityLocation::MSB)
        .value("LSB", hamming_codec::ParityLocation::LSB);

    m.def("encode", &hamming_codec::encode,
            R"pbdoc(
            Hamming encode a data message.

            Performs a single-bit error correcting Hamming encoding
            of the input data of specified length (in number of bits).

            Parameters
            ----------
            data: int
                The data word to encode.
            n_bits: int
                The size of the input data in number of bits.
            parity_location: ParityLocation
                Where to place the parity bits

            Returns
            -------
            str
                The binary string representation of the Hamming encoded data.
            )pbdoc",
            py::arg("data"), py::arg("n_bits"), py::arg("parity_location") = hamming_codec::ParityLocation::DEFAULT
    );

    m.def("decode", &hamming_codec::decode,
            R"pbdoc(
            Decode a Hamming encoded data word.

            Decodes the Hamming encoded input message. This method will
            automatically correct any detected single-bit error.

            Parameters
            ----------
            data: int
                The data word to decode.
            n_bits: int
                The size of the input data in number of bits.
            parity_location: ParityLocation
                Where to look for the parity bits
            n_parity_bits: int
                Number of parity bits to look for if parity bits are in LSB or MSB location

            Returns
            -------
            str
                The binary string representation of the decoded message.
            )pbdoc",
            py::arg("data"), py::arg("n_bits"), py::arg("parity_location") = hamming_codec::ParityLocation::DEFAULT, py::arg("n_parity_bits") = 0
    );

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
