#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include <iostream>

#include "hamming_codec.h"

PYBIND11_MODULE(_hamming_codec, m) {
    m.def("n_parity_bits", &hamming_codec::n_parity_bits_required);
    m.def("compute_parity_bit_positions", &hamming_codec::compute_parity_bit_positions);
    m.def("compute_parity_bits", &hamming_codec::compute_parity_bits);
    m.def("n_parity_bits_required", &hamming_codec::n_parity_bits_required);
    m.def("int2bin", &hamming_codec::int2bin);

    m.def("encode", &hamming_codec::encode, "Encode an input value");
    m.def("decode", &hamming_codec::decode, "Decode an input value");
}
