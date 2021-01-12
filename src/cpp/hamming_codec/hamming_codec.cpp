#include "hamming_codec.h"
#include "utils.h"

//#include <pybind11/embed.h>
//namespace py = pybind11;
#include <iostream>
#include <sstream>
#include <math.h>
#include <algorithm> // std::copy_if, std::reverse
#include <numeric> // std::reduce, std::iota

namespace hamming_codec {

uint32_t n_parity_bits_required(uint32_t n_bits) {
    uint32_t p = 1;
    while(true) {
        uint32_t lhs = static_cast<uint32_t>(pow(2, p));
        uint32_t rhs = p + n_bits + 1;
        if (lhs >= rhs) { break; }
        p += 1;
    }
    return p;
}

std::vector<uint32_t> compute_parity_bits(std::string binary_string, std::vector<unsigned> positions, bool inclusive) {

    std::vector<unsigned> parity_bits(positions.size());

    std::vector<unsigned> range;
    for(size_t i = 0; i < binary_string.length() + 1; i++)
    {
        range.push_back(i);
    }

    for (size_t i = 0; i < positions.size(); i++) {
        unsigned mask = 1 << i;

        auto filter_func = [&](unsigned d) {
            if(inclusive) {
                return (mask & d) != 0;
            } else {
                return ((mask & d) != 0) && (mask != d);
            }
        };
        std::vector<unsigned> r_pos(range.size());
        auto it = std::copy_if(range.begin(), range.end(), r_pos.begin(), filter_func);
        r_pos.resize(std::distance(r_pos.begin(), it));

        std::vector<unsigned> data_sel;
        std::vector<char> binary_string_arr;
        std::copy(binary_string.begin(), binary_string.end(), std::back_inserter(binary_string_arr));
        for(auto d : r_pos) {
            if(binary_string_arr[d-1] == '1') {
                data_sel.push_back(1);
            } else {
                data_sel.push_back(0);
            }
        } // d

        auto fxor = [&](int x, int y) { return x ^ y; };
        auto xor_result = std::reduce(data_sel.begin(), data_sel.end(), 0, fxor);
        if (xor_result == 1) {
            parity_bits[i] = 1;
        }
    } // i
    return parity_bits;
}

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

std::vector<uint32_t> compute_parity_bit_positions(uint32_t n_parity_bits) {
    std::vector<uint32_t> positions;
    for(size_t i = 0; i < n_parity_bits; i++)
    {
        positions.push_back( static_cast<uint32_t>(pow(2, i) - 1) );
    }
    return positions;
}

std::string encode(uint64_t data, uint32_t n_bits)
{
    std::string binary_string = int2bin(data, n_bits);

    auto n_parity_bits = n_parity_bits_required(n_bits);
    auto parity_bit_positions = compute_parity_bit_positions(n_parity_bits);

    unsigned len_encoded_message = n_parity_bits + n_bits;

    std::string binary_string_reversed = binary_string;
    std::reverse_copy(binary_string.begin(), binary_string.end(), binary_string_reversed.begin());

    std::vector<std::string> seed_string_arr;
    for(size_t i = 0; i < len_encoded_message; i++)
    {
        seed_string_arr.push_back("x");
    }

    unsigned data_idx = 0;
    for(size_t i = 0; i < len_encoded_message; i++) {
        if (std::find(parity_bit_positions.begin(), parity_bit_positions.end(), static_cast<uint32_t>(i)) == parity_bit_positions.end()) {
            seed_string_arr[i] = binary_string_reversed[data_idx];
            data_idx++;
        }
    } // i

    std::stringstream ss;
    for(auto s: seed_string_arr) {
        ss << s;
    }
    std::string seed_string = ss.str();

    auto parity_bits = compute_parity_bits(seed_string, parity_bit_positions, false);

    unsigned parity_bit_idx = 0;
    for(unsigned i = 0; i < seed_string_arr.size(); i++) {
        if (seed_string_arr.at(i) == "x") {
            seed_string_arr[i] = std::to_string(parity_bits.at(parity_bit_idx));
            parity_bit_idx++;
        }
    }

    std::stringstream encoded_stream;
    for(auto s : seed_string_arr) {
        encoded_stream << s;
    }
    auto encoded = encoded_stream.str();
    std::reverse(encoded.begin(), encoded.end());
    return encoded;
}

std::string decode(std::string binary_string, uint32_t n_bits)
{
    auto n_parity_bits = n_parity_bits_required(n_bits);
    auto parity_bit_positions = compute_parity_bit_positions(n_parity_bits);
    std::string binary_string_reversed = binary_string;
    std::reverse_copy(binary_string.begin(), binary_string.end(), binary_string_reversed.begin());
    auto parity_bits = compute_parity_bits(binary_string_reversed, parity_bit_positions, true);

    std::stringstream sx;
    for(auto p : parity_bits) sx << p;
    std::string error_string = sx.str();
    std::reverse(error_string.begin(), error_string.end());
    unsigned error_position = std::stoul(error_string, nullptr, 2);

    std::string decoded_string = binary_string_reversed;
    if(error_position > 0) {
        decoded_string[error_position - 1] = (decoded_string[error_position-1] == '0') ? '1' : '0';
    }

    // remove parity bits
    std::stringstream decoded_stream;
    for(size_t i = 0; i < decoded_string.length(); i++) {
        if(std::find(parity_bit_positions.begin(), parity_bit_positions.end(), static_cast<uint32_t>(i)) == parity_bit_positions.end()) {
            decoded_stream << decoded_string[i];
        }
    }
    std::string decoded = decoded_stream.str();
    std::reverse(decoded.begin(), decoded.end());
    return decoded;

}
} // namespace hamming_codec
