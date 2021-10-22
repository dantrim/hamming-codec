#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <numeric> // accumulate

namespace hamming_codec {

    namespace utils {

        // convert an integer into a binary string
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

        template <class T>
        std::string join(const std::vector<T>& vec) {
            std::stringstream sx;
            for(const auto& x: vec) {
                sx << x;
            }
            return sx.str();
        }
        template std::string join<std::string>(const std::vector<std::string>&);
        template std::string join<int>(const std::vector<int>&);

        std::vector<char> split(const std::string& s) {
            //std::vector<char> out(s.length());
            //std::copy(s.begin(), s.end(), out.begin());
            std::vector<char> out(s.begin(), s.end());
            return out;
        }

        std::string reverse_string(const std::string input) {
            std::string out = input;
            std::reverse_copy(input.begin(), input.end(), out.begin());
            return out;
        }

        std::vector<std::string> replace_with(std::vector<std::string>& seed,
                const std::vector<std::string>& fill, const std::string& flag = "x") {
            if(flag.length() != 1) {
                std::stringstream e;
                e << "Input flag string (=" << flag << ") is not length 1";
                throw std::runtime_error(e.str());
            }
            std::vector<std::string> reversed_fill{fill.size()};
            std::reverse_copy(fill.begin(), fill.end(), reversed_fill.begin());
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
    } // namespace utils

    namespace hu = utils;

    //
    // private functions
    //
    namespace internal {

        // compute the number of parity bits required for encoding
        // a message of size n_bits
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

        // determine the bit positions of the parity bits for an encoded message
        // that has n_parity_bits
        std::vector<uint32_t> compute_parity_bit_positions(uint32_t n_parity_bits) {
            std::vector<uint32_t> positions;
            for(size_t i = 0; i < n_parity_bits; i++) {
                positions.push_back(static_cast<uint32_t>(pow(2,i)-1));
            }
            return positions;
        }

        // computes the values of the parity bits
        std::vector<uint32_t> compute_parity_bits(std::string binary_string, std::vector<unsigned> parity_bit_positions, bool inclusive) {
            std::vector<unsigned> parity_bits(parity_bit_positions.size());

            std::vector<unsigned> range;
            for(size_t i = 0; i < binary_string.length() + 1; i++)
            {
                range.push_back(i);
            }

            for (size_t i = 0; i < parity_bit_positions.size(); i++) {
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
                auto binary_string_array = hu::split(binary_string);
                for(auto d : r_pos) {
                    if(binary_string_array[d-1] == '1') {
                        data_sel.push_back(1);
                    } else {
                        data_sel.push_back(0);
                    }
                } // d

                // perform a reduction over the data bits, operating with the XOR (^)
                // over all elements
                auto fxor = [&](int x, int y) { return x ^ y; };
                auto xor_result = std::accumulate(data_sel.begin(), data_sel.end(), 0, fxor);
                if (xor_result == 1) {
                    parity_bits[i] = 1;
                }
            } // i
            return parity_bits;
        }

        // create a "seed" string from which the final encoded message will be
        // constructed, initializing it all with the character 'x'
        std::vector<std::string> create_seed_string_array(std::vector<uint32_t> skip_positions, unsigned length, std::string word_to_split) {
            std::vector<std::string> seed_string_arr{length, "x"};
            unsigned data_idx = 0;
            for(size_t i = 0; i < length; i++) {
                if (std::find(skip_positions.begin(), skip_positions.end(), static_cast<uint32_t>(i)) == skip_positions.end()) {
                    seed_string_arr[i] = word_to_split[data_idx++];
                }
            } // i
            return seed_string_arr;
        }
    } // namespace internal



    // encode the message `data` which should be interpreted as being
    // composed of `n_bits` bits
    std::string encode(uint64_t data, uint32_t n_bits) {
        std::string binary_string = hu::int2bin(data, n_bits);

        // reverse so that string character index 0 corresponds to the LSB
        auto binary_string_reversed = hu::reverse_string(binary_string);

        auto n_parity_bits = internal::n_parity_bits_required(n_bits);
        auto parity_bit_positions = internal::compute_parity_bit_positions(n_parity_bits);

        unsigned len_encoded_message = n_parity_bits + n_bits;

        // create a seed string from which the encoded message will be built
        auto seed_string_array = internal::create_seed_string_array(parity_bit_positions, len_encoded_message, binary_string_reversed);
        auto seed_string = hu::join(seed_string_array);

        // should put a test
        if(seed_string_array.size() != len_encoded_message) {
            std::stringstream e;
            e << "Length of seed string (=" << seed_string_array.size() << ") does not match the expected (=" << len_encoded_message << ")";
            throw std::runtime_error(e.str());
        }

        // compute the parity bits with the "inclusive" flag set to false,
        // meaning to not include the data at the indices of the parity
        // bits when computing a given parity bit
        auto parity_bits = internal::compute_parity_bits(seed_string, parity_bit_positions, false);

        // should put a test
        if(parity_bits.size() != n_parity_bits) {
            std::stringstream e;
            e << "Number of computed parity bits (=" << parity_bits.size() << ") does not match the expected number (=" << n_parity_bits << ")";
            throw std::runtime_error(e.str());
        }

        // update the flagged ("x") characters with the corresponding parity bits
        std::vector<std::string> parity_bit_chars{parity_bits.size()};
        std::transform(parity_bits.begin(), parity_bits.end(), parity_bit_chars.begin(), [](int x) { return std::to_string(x); });
        hu::replace_with(seed_string_array, parity_bit_chars, "x");

        // reverse the now complete seed_string to make the RHS of binary the string
        // correspond to the LSB
        return hu::reverse_string(hu::join(seed_string_array));
    }

    // decode the hamming encoded message `data` which should be interpreted as
    // being `n_bits` long
    std::string decode(uint64_t data, uint32_t n_bits) {
        std::string binary_string = hu::int2bin(data, n_bits);
        auto n_parity_bits = internal::n_parity_bits_required(n_bits);
        auto parity_bit_positions = internal::compute_parity_bit_positions(n_parity_bits);
        auto binary_string_reversed = hu::reverse_string(binary_string);

        // compute the parity bits with "inclusive" set to true, meaning
        // to include the parity bit locations in the computation of a given parity bit
        auto parity_bits = internal::compute_parity_bits(binary_string_reversed, parity_bit_positions, true);

        // the parity bits as a string in the scenario correspond to the position (index starting from 1)
        // at which an error has occurred (a bit has been flipped)
        auto error_string = hu::join(parity_bits);
        std::reverse(error_string.begin(), error_string.end());
        unsigned error_position = std::stoul(error_string, nullptr, 2);

        std::string decoded_string = binary_string_reversed;
        // bit flip the error position
        if(error_position > 0) {
            decoded_string[error_position - 1] = (decoded_string[error_position-1] == '0') ? '1' : '0';
        }

        // remove parity bits from the string
        auto decoded = hu::remove_chars_at(decoded_string, parity_bit_positions);

        // reverse so that the RHS character in the binary string corresponds to the
        // LSB of the decoded data
        std::reverse(decoded.begin(), decoded.end());
        return decoded;
    }

} // namespace hamming_codec
