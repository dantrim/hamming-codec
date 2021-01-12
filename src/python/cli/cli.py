"""hamming CLI"""

import click

from . import codec_cli


@click.group(context_settings=dict(help_option_names=["-h", "--help"]))
@click.option("-v", "--verbose", is_flag=True, default=False)
@click.pass_context
def hamming(ctx, verbose):
    """Top-level entrypoint into hamming-codec utilities."""

    # ensure that ctx.obj exists and is a dict
    ctx.ensure_object(dict)
    # pass the verbose flag to the sub-commands
    ctx.obj["VERBOSE"] = verbose


hamming.add_command(codec_cli.encode)
hamming.add_command(codec_cli.decode)
