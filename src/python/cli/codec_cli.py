"""hamming CLI"""

import typer

from enum import Enum
import sys

import hamming_codec


class ParityLocationChoices(str, Enum):
    default = ("DEFAULT",)
    msb = ("MSB",)
    lsb = "LSB"


def encode(
    ctx: typer.Context,
    input: str = typer.Argument(..., help="Input data word as hex string"),
    n_bits: int = typer.Argument(
        ..., help="Length of the input data word in number of bits"
    ),
    parity_location: ParityLocationChoices = typer.Argument(
        "DEFAULT",
        case_sensitive=False,
        help="Speciy how the parity bits are placed in the encoded message",
    ),
):
    """
    Encode the provided input data word, which is interpreted
    as being word of the specified number of bits.
    """

    # decode the input string as a hexadecimal string
    try:
        input_data = int(input, 16)
    except ValueError:
        print("Input data must be a valid hexadecimal string")
        sys.exit(1)

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
        print(f"Input value         : 0x{input.replace('0x','')}, size = {n_bits} bits")
        print(f"Input value (bin)   : 0b{input_data_binary_string}")
        print(f"Encoded value       : {hex(encoded_int)}")
        print(
            f"Encoded value (bin) : 0b{encoded_binary_string}, size = {len(encoded_binary_string)} bits"
        )
    else:
        print(f"{hex(encoded_int)} {len(encoded_binary_string)}", file=sys.stdout)


def decode(
    ctx: typer.Context,
    input: str = typer.Argument(..., help="Input message to decode as a hex string"),
    n_bits: int = typer.Argument(
        ..., help="Length of the input message word in number of bits"
    ),
    parity_location: ParityLocationChoices = typer.Argument(
        "DEFAULT",
        case_sensitive=False,
        help="Specify how the parity bits are placed in the encoded message",
    ),
    n_parity_bits: int = typer.Argument(
        0,
        help="Number of parity bits in the message (required for non-default parity location choice)",
    ),
):
    """
    Decode the input message that is the specified number of bits in
    length.
    """

    # input string as a hexadecimal string
    try:
        input_data = int(input, 16)
    except ValueError:
        print("Input data must be a valid hexadecimal string")
        sys.exit(1)

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

    if n_parity_bits == 0 and parity_location != hamming_codec.ParityLocation.DEFAULT:
        raise ValueError(
            "For non-default parity bit locations, the number of parity bits  must be specified"
        )

    # decode
    decoded_binary_string = hamming_codec.decode(
        input_data, n_bits_input, parity_location, n_parity_bits
    )
    decoded_int = int(decoded_binary_string, 2)
    if ctx.obj["VERBOSE"]:
        print(
            f"Input value         : 0x{input.replace('0x','')}, size = {n_bits_input} bits"
        )
        print(f"Input value (bin)   : 0b{input_data_binary_string}")
        print(f"Decoded value       : {hex(decoded_int)}")
        print(
            f"Decoded value (bin) : 0b{decoded_binary_string}, size = {len(decoded_binary_string)} bits"
        )
    else:
        print(f"{hex(decoded_int)} {len(decoded_binary_string)}", file=sys.stdout)
