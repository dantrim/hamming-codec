#include <string>
#include <vector>

namespace hamming_codec {
    uint32_t n_parity_bits_required(uint32_t n_bits);
    std::vector<uint32_t> compute_parity_bit_positions(uint32_t n_parity_bits);
    std::vector<uint32_t> compute_parity_bits(std::string binary_string, std::vector<unsigned> parity_bit_positions, bool inclusive);
    std::vector<std::string> create_seed_string_array(std::vector<uint32_t> skip_positions, unsigned length, std::string word_to_split);

    std::string encode(uint64_t data, uint32_t n_bits);
    std::string decode(uint64_t data, uint32_t n_bits);
} // namespace hamming_codec
