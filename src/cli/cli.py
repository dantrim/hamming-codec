"""hamming CLI"""

import click

from . import codec_cli


@click.group(context_settings=dict(help_option_names=["-h", "--help"]))
def hamming():
    """Top-level entrypoint into hamming-codec utilities."""


hamming.add_command(codec_cli.encode)
hamming.add_command(codec_cli.decode)
