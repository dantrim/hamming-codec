"""hamming CLI"""

import click
import sys

import hamming_codec


@click.group(name="codec")
def cli():
    """The codec CLI group."""


@cli.command()
@click.pass_context
@click.argument("input", type=str)
@click.argument("n-bits", type=int)
@click.argument("parity-location", type=str, default="DEFAULT")
def encode(ctx, input, n_bits, parity_location):
    """
    Encode the provided input data word, which is interpreted
    as being word of the specified number of bits.
    """
    # decode the input string as a hexadecimal string
    input_data = int(input, 16)
    n_bits = int(n_bits)
    if n_bits < 4:
        raise ValueError("Cannot encode values that are less than 4 bits in length!")
    # convert to binary string of `n_bits` bits
    input_data_binary_string = f"{bin(input_data)[2:]:0>{n_bits}}"
    parity_location_map = {
        "DEFAULT": hamming_codec.ParityLocation.DEFAULT,
        "MSB": hamming_codec.ParityLocation.MSB,
        "LSB": hamming_codec.ParityLocation.LSB,
    }
    if parity_location not in parity_location_map:
        raise ValueError(f'Invalid parity location provided: "{parity_location}"')
    parity_location = parity_location_map[parity_location]

    # encode
    encoded_binary_string = hamming_codec.encode(input_data, n_bits, parity_location)
    encoded_int = int(encoded_binary_string, 2)
    if ctx.obj["VERBOSE"]:
        print(f"Input value         : 0x{input}, size = {n_bits} bits")
        print(f"Input value (bin)   : 0b{input_data_binary_string}")
        print(f"Encoded value       : 0x{hex(encoded_int)[2:]}")
        print(
            f"Encoded value (bin) : 0b{encoded_binary_string}, size = {len(encoded_binary_string)} bits"
        )
    else:
        print(f"0x{hex(encoded_int)[2:]} {len(encoded_binary_string)}", file=sys.stdout)


@cli.command()
@click.pass_context
@click.argument("input", type=str)
@click.argument("n-bits", type=int)
@click.argument("parity-location", type=str, default="DEFAULT")
@click.argument("n-parity-bits", type=int, default=0)
def decode(ctx, input, n_bits, parity_location, n_parity_bits):
    """
    Decode the input message that is the specified number of bits in
    length.
    """
    # input string as a hexadecimal string
    input_data = int(input, 16)
    n_bits_input = int(n_bits)
    if n_bits_input < 4:
        raise ValueError("Cannot decode values that are less than 4 bits in length!")

    # convert to binary string of `n_bits_input` bits
    input_data_binary_string = f"{bin(input_data)[2:]:0>{n_bits_input}}"

    parity_location_map = {
        "DEFAULT": hamming_codec.ParityLocation.DEFAULT,
        "MSB": hamming_codec.ParityLocation.MSB,
        "LSB": hamming_codec.ParityLocation.LSB,
    }
    if parity_location not in parity_location_map:
        raise ValueError(f'Invalid parity location provided: "{parity_location}"')
    parity_location = parity_location_map[parity_location]

    # decode
    decoded_binary_string = hamming_codec.decode(
        input_data, n_bits_input, parity_location, n_parity_bits
    )
    decoded_int = int(decoded_binary_string, 2)
    if ctx.obj["VERBOSE"]:
        print(f"Input value         : 0x{input}, size = {n_bits_input} bits")
        print(f"Input value (bin)   : 0b{input_data_binary_string}")
        print(f"Decoded value       : 0x{hex(decoded_int)[2:]}")
        print(
            f"Decoded value (bin) : 0b{decoded_binary_string}, size = {len(decoded_binary_string)} bits"
        )
    else:
        print(f"0x{hex(decoded_int)[2:]} {len(decoded_binary_string)}", file=sys.stdout)
