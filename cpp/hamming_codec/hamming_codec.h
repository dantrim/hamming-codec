#include <string>

namespace hamming_codec {
    std::string encode(uint64_t data, uint32_t n_bits);
    std::string decode(std::string binary_string, uint32_t n_bits);
    std::string int2bin(uint64_t input, uint32_t n_bits);
} // namespace hamming_codec
