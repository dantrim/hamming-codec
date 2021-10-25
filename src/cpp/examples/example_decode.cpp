// std/stl
#include <iostream>
#include <map>
#include <getopt.h>

// hamming codec wrapper
#include "hamming_codec.h"

void usage(char* argv[]) {
    std::cerr << "Usage: " << argv[0] << " [OPTIONS] INPUT N_BITS [PARITY_LOC] [N_PARITY_BITS]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "OPTIONS:" << std::endl;
    std::cerr << " -h|--help    Print this help message." << std::endl;
    std::cerr << std::endl;
    std::cerr << "POSITIONAL ARGUMENTS:" << std::endl;
    std::cerr << " INPUT          Input data to be decoded (as hexadecimal string)" << std::endl;
    std::cerr << " N_BITS         Size, in number of bits, of the input data" << std::endl;
    std::cerr << " PARITY_LOC     Where the parity bits are located in the encoded message [DEFAULT|MSB|LSB]" << std::endl;
    std::cerr << " N_PARITY_BITS  Number of parity bits contained in encoded message (only if MSB|LSB)" << std::endl;
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
    if(n_pos > 4) {
        std::cerr << "Too many positional arguments provided" << std::endl;
        return 1;
    }
    if(!(n_pos == 2 || n_pos == 4)) {
        std::cerr << "Missing positional arguments!" << std::endl;
        return 1;
    }

    std::string input_data_string = argv[optind++];
    uint32_t n_bits = std::stoul(argv[optind++]);;
    uint32_t n_parity_bits = 0;
    std::string parity_loc_str = "DEFAULT";
    if(n_pos >= 3) {
        parity_loc_str = argv[optind++];
        if(n_pos != 4) {
            std::cerr << "Missing specification of PARITY_LOC|N_PARITY_BITS" << std::endl;
            return 1;
        }
        n_parity_bits = std::stoul(argv[optind++]);
    }

    if(n_bits<4) {
        throw std::runtime_error("Cannot decode values that are less than 4 bits in length!");
    }
    uint64_t input_data = std::stoul(input_data_string, 0, 16);
    auto input_data_binary_string = hamming_codec::utils::int2bin(input_data, n_bits);

    hamming_codec::ParityLocation parity_location{hamming_codec::ParityLocation::DEFAULT};
    std::map<std::string, hamming_codec::ParityLocation> parity_loc_map{
        {"DEFAULT", hamming_codec::ParityLocation::DEFAULT},
        {"MSB", hamming_codec::ParityLocation::MSB},
        {"LSB", hamming_codec::ParityLocation::LSB}
    };
    if(parity_loc_map.count(parity_loc_str) == 0) {
        throw std::runtime_error("Invalid parity location provided: \"" + parity_loc_str + "\"");
    }

    // decode
    std::string decoded_binary_string = hamming_codec::decode(input_data, n_bits, parity_loc_map.at(parity_loc_str), n_parity_bits);
    uint64_t decoded_int = std::stoul(decoded_binary_string, 0, 2);
    if(verbose) {
        std::cout << "Input value         : 0x" << std::hex << input_data << std::dec << ", size = " << n_bits << " bits" << std::endl;
        std::cout << "Input value (bin)   : 0b" << input_data_binary_string << std::endl;
        std::cout << "Decoded value       : 0x" << std::hex << decoded_int << std::dec << std::endl;
        std::cout << "Decoded value (bin) : 0b" << decoded_binary_string << ", size = " << decoded_binary_string.length() << " bits" << std::endl;
    } else {
        std::cout << "0x" << std::hex << decoded_int << std::dec << " " << decoded_binary_string.length() << std::endl;
    }
    return 0;
}
