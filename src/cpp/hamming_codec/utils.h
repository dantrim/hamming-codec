#include <string>
#include <vector>
#include <sstream>

namespace hamming_codec {
namespace utils {

    std::string int2bin(uint64_t data, uint32_t n_bits);

    template <class T>
    std::string join(std::vector<T> vec) {
        std::stringstream sx;
        for(const auto & x : vec) {
            sx << x;
        }
        return sx.str();
    }
    template std::string join<std::string>(std::vector<std::string>);
    template std::string join<int>(std::vector<int>);

    std::vector<char> split(std::string s);

    std::string reverse_string(std::string input);

    std::vector<std::string> replace_with(std::vector<std::string>& seed, const std::vector<std::string>& fill, const std::string& flag = "x");
    std::string remove_chars_at(const std::string& init, const std::vector<unsigned>& positions);
}
}
