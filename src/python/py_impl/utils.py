from functools import reduce
import operator as op


def n_parity_bits_required(n_bits: int) -> int:

    p = 1
    while True:
        lhs = 2 ** p
        rhs = p + n_bits + 1
        if lhs >= rhs:
            break
        p += 1
    return p


def compute_parity_bits(binary_string: str, positions: list, inclusive: bool) -> list:

    parity_bits = [0 for _ in positions]

    for i, p in enumerate(positions):
        mask = 1 << i
        filter_func = {
            False: lambda d: (mask & d != 0) and (mask != d),
            True: lambda d: (mask & d != 0),
        }[inclusive]
        r_pos = [x for x in list(filter(filter_func, range(len(binary_string) + 1)))]
        data_sel = [int(list(binary_string)[d - 1], 2) for d in r_pos]
        xor = reduce(op.xor, data_sel)
        if xor == 1:
            parity_bits[i] = 1
    return parity_bits


def encode(data: int, n_bits: int) -> str:

    binary_string = bin(data)[2:]

    if n_bits < len(binary_string):
        print(
            f"ERROR: Requested data size ({n_bits} bits) is smaller than the binary representation of the input data (={data})"
        )
        return -1
    # pad
    binary_string = f"{binary_string:0>{n_bits}}"

    # parity bits are at powers of 2
    n_parity_bits = n_parity_bits_required(n_bits)
    parity_bit_positions = [2 ** i - 1 for i in range(n_parity_bits)]

    len_encoded_message = n_parity_bits + n_bits
    parity_bit_positions = list(
        filter(lambda x: x < len_encoded_message, parity_bit_positions)
    )

    binary_string_reversed = "".join(reversed(binary_string))

    # placeholder string
    seed_string = "".join(["x" for _ in range(len_encoded_message)])
    seed_string = list(seed_string)

    data_idx = 0
    for idx in range(len_encoded_message):
        if idx not in parity_bit_positions:
            seed_string[idx] = list(binary_string_reversed)[data_idx]
            data_idx += 1
    seed_string = "".join(seed_string)

    # compute the values for the parity bits
    parity_bits = compute_parity_bits(seed_string, parity_bit_positions, False)

    # emplace the values of the parity bits in the flagged positions
    parity_bit_idx = 0
    encoded_string = list(seed_string)
    for i, v in enumerate(encoded_string):
        if v.lower() == "x":
            encoded_string[i] = parity_bits[parity_bit_idx]
            parity_bit_idx += 1
    encoded_string = "".join(map(str, reversed(encoded_string)))
    return encoded_string


def decode(binary_string: str, n_bits: int) -> str:

    # binary string must not have the "0b" preceding characters

    n_parity_bits = n_parity_bits_required(n_bits)
    parity_bit_positions = [2 ** i - 1 for i in range(n_parity_bits)]
    parity_bit_positions = list(
        filter(lambda x: x < len(binary_string), parity_bit_positions)
    )

    binary_string_reversed = "".join(reversed(list(binary_string)))
    parity_bits = compute_parity_bits(
        binary_string_reversed, parity_bit_positions, True
    )
    error_position = int("".join(reversed(list(map(str, parity_bits)))), 2)
    decoded_string = list(binary_string_reversed)
    if error_position > 0:
        # flip the bit at the index where the error is located
        decoded_string[error_position - 1] = {"0": "1", "1": "0"}[
            decoded_string[error_position - 1]
        ]

    # remove the parity bits from the decoded string to get the message data
    decoded_string = [
        v for i, v in enumerate(decoded_string) if i not in parity_bit_positions
    ]
    decoded_string = "".join(reversed(decoded_string))
    return decoded_string
