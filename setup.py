import sys
import pathlib

import os
import subprocess
from setuptools.command.sdist import sdist
from setuptools.command.install import install

# from pybind11 import get_cmake_dir
from pybind11.setup_helpers import Pybind11Extension, build_ext

# from setuptools import setup

# the directory containing this file
HERE = pathlib.Path(__file__).parent

# the text of the README file
README = (HERE / "README.md").read_text()

__version__ = "0.3.1"


try:
    from skbuild import setup
except ImportError:
    print(
        "Please update pip, you need pip 10 or greater,\n"
        " or you need to install the PEP 518 requirements in pyproject.toml yourself",
        file=sys.stderr,
    )
    raise

ext_modules = [
    Pybind11Extension(
        "_hamming_codec",
        ["src/python/module.cpp"],
        include_dirs=["src/cpp"],
        # Example: passing in the version to the compiled code
        define_macros=[("VERSION_INFO", __version__), ("BUILD_PYTHON", "on")],
    ),
]


def check_submodules():
    """verify that the submodules are checked out and clean
    use `git submodule update --init`; on failure
    """
    if not os.path.exists(".git"):
        return
    with open(".gitmodules") as f:
        for line in f:
            if "path" in line:
                p = line.split("=")[-1].strip()
                if not os.path.exists(p):
                    raise ValueError("Submodule {} missing".format(p))

    proc = subprocess.Popen(["git", "submodule", "status"], stdout=subprocess.PIPE)
    status, _ = proc.communicate()
    status = status.decode("ascii", "replace")
    for line in status.splitlines():
        if line.startswith("-") or line.startswith("+"):
            raise ValueError("Submodule not clean: {}".format(line))


class SdistChecker(sdist):
    """ check submodules on sdist to prevent incomplete tarballs """

    def run(self):
        check_submodules()
        sdist.run(self)


class InstallChecker(install):
    """ check submodules on install to prevent failed installs """

    def run(self):
        check_submodules()
        install.run(self)


cmake_compiler_defines = f"-DBUILD_PYTHON=on -DVERSION_INFO={__version__}"
print(f"cmake compiler defines = {cmake_compiler_defines}")

setup(
    name="hamming_codec",
    version=__version__,
    description="Simple encode/decode utilities for bit-error correcting Hamming codes",
    long_description=README,
    long_description_content_type="text/markdown",
    url="https://github.com/dantrim/hamming-codec",
    author="Daniel Joseph Antrim",
    author_email="dantrim1023@gmail.com",
    license="MIT",
    classifiers=[
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.8",
    ],
    package_dir={"": "src/python"},
    packages=["hamming_codec", "cli"],
    install_requires=["typer"],
    ext_modules=ext_modules,
    cmake_args=["-DBUILD_PYTHON=on"],
    cmake_install_dir="src/python/hamming_codec",
    entry_points={"console_scripts": ["hamming=cli:cli.app"]},
    cmdclass={"build_ext": build_ext, "install": InstallChecker, "sdist": SdistChecker},
    include_package_data=True,
)
