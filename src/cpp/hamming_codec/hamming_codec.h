#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <numeric> // accumulate
#include <math.h>

#include <iomanip>
namespace hamming_codec {

    enum class ParityLocation {
        DEFAULT
       ,MSB
       ,LSB
    };

    namespace utils {

        // convert an integer into a binary string
        std::string int2bin(const uint64_t& data, const uint32_t& n_bits) {

            std::string binary_string = "";
            uint64_t mask = 1;
            for(uint32_t i = 0; i < n_bits; i++) {
                if((mask & data) >= 1) {
                    binary_string = "1" + binary_string;
                } else {
                    binary_string = "0" + binary_string;
                }
                std::cout << "FOO int2bin[" << i << "] = 0x" << std::hex << std::stoul(binary_string, 0, 2) << ")" << std::dec << std::endl;
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

        std::string reverse_string(const std::string& input) {
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

        std::string remove_chars_at(const std::string& init, const std::vector<uint64_t>& positions) {
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
        uint64_t n_parity_bits_required(const uint64_t& n_bits) {
            for(uint64_t i = 0; i < n_bits; i++) {
                if(pow(2, i) >= n_bits + i + 1) {
                    return i;
                }
            }
            throw std::runtime_error("Failed to compute number of required parity bits");
            uint64_t p = 1;
            while(true) {
                uint64_t lhs = static_cast<uint64_t>(pow(2, p));
                uint64_t rhs = p + n_bits + 1;
                if (lhs >= rhs) { break; }
                p += 1;
            }
            return p;
        }

        // determine the bit positions of the parity bits for an encoded message
        // that has n_parity_bits
        std::vector<uint64_t> compute_parity_bit_positions(const uint32_t& n_parity_bits) {
            std::vector<uint64_t> positions;
            for(size_t i = 0; i < n_parity_bits; i++) {
                positions.push_back(static_cast<uint64_t>(pow(2,i)-1));
            }
            return positions;
        }

        // computes the values of the parity bits
        std::vector<uint32_t> compute_parity_bits(const std::string& binary_string, const std::vector<uint64_t>& parity_bit_positions, const bool& inclusive) {
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
                        return (mask != d) && ((mask & d) != 0);
                        //return ((mask & d) != 0) && (mask != d);
                    }
                };

                // fill a vector of the positions of the data bits over which the parity should be computed
                std::vector<unsigned> r_pos(range.size());
                auto it = std::copy_if(range.begin(), range.end(), r_pos.begin(), filter_func);
                r_pos.resize(std::distance(r_pos.begin(), it));

                // convert the binary string to a vector of 0's and 1's
                std::vector<unsigned> data_sel;
                auto binary_string_array = hu::split(binary_string);
                for(const auto& pos : r_pos) {
                    data_sel.push_back( binary_string_array[pos-1] == '1' ? 1 : 0 );
                } // pos

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
        // constructed, leaving the parity bit positions as the character 'x'
        // and the rest of the positions the data message provided in 'word_to_split'
        std::vector<std::string> create_seed_string_array(const std::vector<uint64_t>& skip_positions, const unsigned& length, const std::string& word_to_split) {
            std::vector<std::string> seed_string_arr{length, "x"};
            unsigned data_idx = 0;
            for(unsigned i = 0; i < length; i++) {
                if (std::find(skip_positions.begin(), skip_positions.end(), i) == skip_positions.end()) {
                    seed_string_arr[i] = word_to_split[data_idx++];
                }
            } // i
            return seed_string_arr;
        }

    } // namespace internal


    // encode the message `data` which should be interpreted as being
    // composed of `n_bits` bits
    std::string encode(const uint64_t& data, const uint32_t& n_bits, const ParityLocation& parity_loc = ParityLocation::DEFAULT) {
        std::string binary_string = hu::int2bin(data, n_bits);

        // reverse so that string character index 0 corresponds to the LSB
        std::string binary_string_reversed = hu::reverse_string(binary_string);

        uint32_t n_parity_bits = internal::n_parity_bits_required(n_bits);
        std::vector<uint64_t> parity_bit_positions = internal::compute_parity_bit_positions(n_parity_bits);

        unsigned len_encoded_message = n_parity_bits + n_bits;

        // create a seed string from which the encoded message will be built
        std::vector<std::string> seed_string_array = internal::create_seed_string_array(parity_bit_positions, len_encoded_message, binary_string_reversed);
        std::string seed_string = hu::join(seed_string_array);

        // should put a test
        if(seed_string_array.size() != len_encoded_message) {
            std::stringstream e;
            e << "Length of seed string (=" << seed_string_array.size() << ") does not match the expected (=" << len_encoded_message << ")";
            throw std::runtime_error(e.str());
        }

        // compute the parity bits with the "inclusive" flag set to false,
        // meaning to not include the data at the indices of the parity
        // bits when computing a given parity bit
        std::vector<uint32_t> parity_bits = internal::compute_parity_bits(seed_string, parity_bit_positions, false);

        // should put a test
        if(parity_bits.size() != n_parity_bits) {
            std::stringstream e;
            e << "Number of computed parity bits (=" << parity_bits.size() << ") does not match the expected number (=" << n_parity_bits << ")";
            throw std::runtime_error(e.str());
        }

        // update the flagged ("x") characters with the corresponding parity bits
        std::vector<std::string> parity_bit_chars{parity_bits.size()};
        std::transform(parity_bits.begin(), parity_bits.end(), parity_bit_chars.begin(), [](int x) { return std::to_string(x); });


        std::string encoded_output_string{""};
        switch(parity_loc) {
            case ParityLocation::DEFAULT : {
                                               hu::replace_with(seed_string_array, parity_bit_chars, "x");
                                               encoded_output_string = hu::reverse_string(hu::join(seed_string_array));
                                               break;
                                           }
            case ParityLocation::MSB : {
                                           seed_string_array.erase(std::remove(seed_string_array.begin(), seed_string_array.end(), "x"), seed_string_array.end());
                                           for(const auto& p : parity_bit_chars) {
                                               seed_string_array.push_back(p);
                                           }
                                           encoded_output_string = hu::reverse_string(hu::join(seed_string_array));
                                           break;
                                       }
            case ParityLocation::LSB : {
                                           seed_string_array.erase(std::remove(seed_string_array.begin(), seed_string_array.end(), "x"), seed_string_array.end());

                                           for(const auto& p : parity_bit_chars) {
                                               encoded_output_string += std::string(p);
                                           }
                                           encoded_output_string = hu::reverse_string(encoded_output_string);
                                           encoded_output_string += hu::join(seed_string_array);
                                           encoded_output_string = hu::reverse_string(encoded_output_string);
                                           break;
                                       }
        } // switch
        //hu::replace_with(seed_string_array, parity_bit_chars, "x");

        // reverse the now complete seed_string to make the RHS of binary the string
        // correspond to the LSB
        //return hu::reverse_string(hu::join(seed_string_array));
        return encoded_output_string;
    }

    // decode the hamming encoded message `data` which should be interpreted as
    // being `n_bits` long
    std::string decode(const uint64_t& data, const uint32_t& n_bits, const ParityLocation& parity_loc = ParityLocation::DEFAULT) {
        std::string binary_string = hu::int2bin(data, n_bits);
        uint32_t n_parity_bits = internal::n_parity_bits_required(n_bits);
        std::vector<uint64_t> parity_bit_positions = internal::compute_parity_bit_positions(n_parity_bits);
                                          
        //n_parity_bits = (n_bits % 2 == 0) ? n_parity_bits : (n_parity_bits - 1);
        std::cout << "FOO data          = 0x" << std::hex << data << std::dec << " (n_bits = " << n_bits << ")"<<  std::endl;
        std::cout << "FOO binary string = " << binary_string << " (length = " << binary_string.length() << ")" << std::endl;
        std::cout << "FOO n_parity_bits = " << n_parity_bits << std::endl;
        

        // put the parity bits in the "canonical" locations if the encoded data message
        // was not encoded in the "canonical" way
        switch(parity_loc) {
            case ParityLocation::MSB : {
                                           std::string data_without_parity = binary_string.substr(0 + (n_parity_bits+1), binary_string.length());
                                           std::cout << "FOO data_without_parity = " << data_without_parity << " (0x" << std::hex << std::stoul(data_without_parity, 0, 2) << ")" << std::endl;
                                           uint64_t d = std::stoul(data_without_parity, 0, 2);
                                           binary_string = encode(d, data_without_parity.length());
                                           break;
                                       }
            case ParityLocation::LSB : {
                                           std::cout << "FOO input               = " << binary_string << std::endl;
                                           std::string data_without_parity = binary_string.substr(0, binary_string.length() - (n_parity_bits));
                                           std::cout << "FOO data_without_parity = " << data_without_parity << " (0x" << std::hex << std::stoul(data_without_parity, 0, 2) << ")" << std::dec << " len = " << data_without_parity.length() << std::endl;
                                           uint64_t d = std::stoul(data_without_parity, 0, 2);
                                           binary_string = encode(d, data_without_parity.length());
                                           break;
                                       }
            case ParityLocation::DEFAULT : {
                                               break;
                                           }
        } // switch

        std::string binary_string_reversed = hu::reverse_string(binary_string);

        // compute the parity bits with "inclusive" set to true, meaning
        // to include the parity bit locations in the computation of a given parity bit
        std::vector<uint32_t> parity_bits = internal::compute_parity_bits(binary_string_reversed, parity_bit_positions, true);

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
