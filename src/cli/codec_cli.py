"""hamming CLI"""

import click

import hamming_codec.utils as utils


@click.group(name="codec")
def cli():
    """The codec CLI group."""


@cli.command()
@click.argument("input", type=str)
@click.argument("n-bits", type=int)
def encode(input, n_bits):
    """
    Encode the provided input data word, which is interpreted
    as being word of the specified number of bits.
    """
    # decode the input string as a hexadecimal string
    input_data = int(input, 16)
    n_bits = int(n_bits)
    # convert to binary string of `n_bits` bits
    input_data_binary_string = f"{bin(input_data)[2:]:0>{n_bits}}"
    # encode
    encoded_binary_string = utils.encode(input_data, n_bits)
    encoded_int = int(encoded_binary_string, 2)
    print(f"Input value         : 0x{input}, size = {n_bits} bits")
    print(f"Input value (bin)   : 0b{input_data_binary_string}")
    print(f"Encoded value       : 0x{hex(encoded_int)[2:]}")
    print(
        f"Encoded value (bin) : 0b{encoded_binary_string}, size = {len(encoded_binary_string)} bits"
    )


@cli.command()
@click.argument("input", type=str)
@click.argument("n-bits", type=int)
def decode(input, n_bits):
    """
    Decode the input message that is the specified number of bits in
    length.
    """
    # input string as a hexadecimal string
    input_data = int(input, 16)
    n_bits_input = int(n_bits)
    # convert to binary string of `n_bits_input` bits
    input_data_binary_string = f"{bin(input_data)[2:]:0>{n_bits_input}}"
    # decode
    decoded_binary_string = utils.decode(input_data_binary_string, n_bits_input)
    decoded_int = int(decoded_binary_string, 2)
    print(f"Input value         : 0x{input}, size = {n_bits_input} bits")
    print(f"Input value (bin)   : 0b{input_data_binary_string}")
    print(f"Decoded value       : 0x{hex(decoded_int)[2:]}")
    print(
        f"Decoded value (bin) : 0b{decoded_binary_string}, size = {len(decoded_binary_string)} bits"
    )
