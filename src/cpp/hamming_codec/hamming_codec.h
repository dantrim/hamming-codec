#include <string>
#include <vector>

namespace hamming_codec {
    std::string int2bin(uint64_t input, uint32_t n_bits);
    uint32_t n_parity_bits_required(uint32_t n_bits);
    std::vector<uint32_t> compute_parity_bit_positions(uint32_t n_parity_bits);
    std::vector<uint32_t> compute_parity_bits(std::string binary_string, std::vector<unsigned> parity_bit_positions, bool inclusive);

    std::string encode(uint64_t data, uint32_t n_bits);
    std::string decode(std::string binary_string, uint32_t n_bits);
} // namespace hamming_codec
