#include "utils.h"
#include <algorithm>
#include <iostream>

namespace hamming_codec {
namespace utils {

    std::string int2bin(uint64_t data, uint32_t n_bits) {
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

    std::vector<char> split(std::string s) {
        std::vector<char> out(s.length());
        std::copy(s.begin(), s.end(), out.begin());
        return out;
    }
    std::string reverse_string(std::string input) {
        std::string out = input;
        std::reverse_copy(input.begin(), input.end(), out.begin());
        return out;
    }

    std::vector<std::string> replace_with(std::vector<std::string>& seed, const std::vector<std::string>& fill_vals, const std::string& flag) {

        if(flag.length() != 1) {
            std::stringstream e;
            e << "Input flag string (=" << flag << ") is not length 1";
            throw std::runtime_error(e.str());
        }
        std::vector<std::string> reversed_fill{fill_vals.size()};
        std::reverse_copy(fill_vals.begin(), fill_vals.end(), reversed_fill.begin());
        auto fp = [&reversed_fill, flag](std::string ch) {
               if(ch == flag) {
                   auto new_val = reversed_fill.back();
                   reversed_fill.pop_back();
                   return new_val;
               } else {
                   return ch;
               }
        };
        std::transform(seed.begin(), seed.end(), seed.begin(), fp);
        return seed;
    }

    std::string remove_chars_at(const std::string& init, const std::vector<unsigned>& positions) {
        std::stringstream output;
        for(size_t i = 0; i < init.size(); i++) {
            if(std::find(positions.begin(), positions.end(), static_cast<uint32_t>(i)) == positions.end()) {
                output << init[i];
            }
        }
        return output.str();
    }
}
}
