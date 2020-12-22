#include "hamming_codec.h"
#include "utils.h"

#include <pybind11/embed.h>
namespace py = pybind11;
#include <iostream>
#include <sstream>

namespace hamming_codec {
std::string int2bin(uint64_t data, uint32_t n_bits)
{
    std::string binary_string = "";
    int mask = 1;
    for(uint32_t i = 0; i < n_bits; i++) {
        if((mask & data) >= 1) {
            binary_string = "1" + binary_string;
        } else {
            binary_string = "0" + binary_string;
        }
        mask = mask << 1;
    } // for
    return binary_string;
}

std::string encode(uint64_t data, uint32_t n_bits)
{
    py::scoped_interpreter guard;

    // a kludge to get things working with virtual environments
    py::module_ sys = py::module_::import("sys");
    sys.attr("path")
        .attr("insert")
        (0, hamming_codec::utils::python_path());

    auto hamming_codec = py::module::import("hamming_codec");
    py::object result = hamming_codec.attr("encode")(data, n_bits);
    auto binary_string = result.cast<std::string>();
    return binary_string;
}

std::string decode(std::string binary_string, uint32_t n_bits)
{
    py::scoped_interpreter guard;

    // a kludge to get things working with virtual environments
    py::module_ sys = py::module_::import("sys");
    sys.attr("path")
        .attr("insert")
        (0, hamming_codec::utils::python_path());

    auto hamming_codec = py::module::import("hamming_codec");
    py::object result = hamming_codec.attr("decode")(binary_string, n_bits);
    auto binary_string_decoded = result.cast<std::string>();
    return binary_string_decoded;
}
} // namespace hamming_codec


