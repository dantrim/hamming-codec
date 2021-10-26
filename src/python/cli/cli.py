"""hamming CLI"""

import typer
import sys

from . import codec_cli

app = typer.Typer()


@app.callback()
def hamming(
    ctx: typer.Context,
    verbose: bool = typer.Option(False, "-v", "--verbose", help="Verbose output"),
):
    """Top-level entrypoint into hamming-codec commandline utilities"""

    if ctx.invoked_subcommand is None:
        print("No subcommand specified")
        sys.exit(1)

    # ensure that ctx.obj exists and is a dict
    ctx.ensure_object(dict)

    # pass the verbose flag to the sub-commands
    ctx.obj["VERBOSE"] = verbose


app.command()(codec_cli.encode)
app.command()(codec_cli.decode)
