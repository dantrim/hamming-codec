// std/stl
#include <iostream>
#include <getopt.h>
#include <cstdlib>
#include <sstream>

#include "hamming_codec.h"
#include "utils.h"

void usage(char* argv[]) {
    std::cerr << "Usage: " << argv[0] << " [OPTIONS] INPUT N_BITS" << std::endl;
    std::cerr << std::endl;
    std::cerr << "OPTIONS:" << std::endl;
    std::cerr << " -h|--help    Print this help message." << std::endl;
    std::cerr << std::endl;
    std::cerr << "POSITIONAL ARGUMENTS:" << std::endl;
    std::cerr << " INPUT        Input data to be encoded (as hexadecimal string)" << std::endl;
    std::cerr << " N_BITS       Size, in number of bits, of the input data" << std::endl;
}


int main(int argc, char* argv[]) {

    bool verbose = false;

    struct option longopts_t[]{{"verbose", no_argument,  NULL, 'v'},{"help",no_argument,NULL,'h'},{0,0,0,0}};
    int c;
    while((c=getopt_long(argc, argv, "vh", longopts_t, NULL)) != -1) {
        switch(c) {
            case 'v':
                verbose = true;
                break;
            case 'h':
                usage(argv);
                return 0;
                break;
            case '?':
                std::cerr << "Invalid option \"" << c << "\" provided" << std::endl;
                return 1;
        } // switch
    } // while
    auto n_pos = (argc - optind);
    if(n_pos > 2) {
        std::cerr << "Too many positional arguments provided" << std::endl;
        return 1;
    }
    if(n_pos != 2) {
        std::cerr << "Missing positional arguments!" << std::endl;
        return 1;
    }

    std::string input_data_string = argv[optind++];
    uint32_t n_bits = std::stoul(argv[optind++]);;

    if(n_bits<4) {
        throw std::runtime_error("Cannot encode values that are less than 4 bits in length!");
    }

    uint64_t input_data = std::stoul(input_data_string, 0, 16);
    auto input_data_binary_string = hamming_codec::utils::int2bin(input_data, n_bits);

    // encode
    auto encoded_binary_string = hamming_codec::encode(input_data, n_bits);
    uint64_t encoded_int = std::stoul(encoded_binary_string, nullptr, 2);
    if(verbose) {
        std::cout << "Input value         : 0x" << std::hex << input_data << std::dec << ", size = " << n_bits << " bits" << std::endl;
        std::cout << "Input value (bin)   : 0b" << input_data_binary_string << std::endl;
        std::cout << "Encoded value       : 0x" << std::hex << encoded_int << std::dec << std::endl;
        std::cout << "Encoded value (bin) : 0b" << encoded_binary_string << ", size = " << encoded_binary_string.length() << " bits" << std::endl;
    } else {
        std::cout << "0x" << std::hex << encoded_int << std::dec << " " << encoded_binary_string.length() << std::endl;
    }
    return 0;
}
