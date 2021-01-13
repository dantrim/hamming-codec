import sys
import pathlib

# the directory containing this file
HERE = pathlib.Path(__file__).parent

# the text of the README file
README = (HERE / "README.md").read_text()

try:
    from skbuild import setup
except ImportError:
    print(
        "Please update pip, you need pip 10 or greater,\n"
        " or you need to install the PEP 518 requirements in pyproject.toml yourself",
        file=sys.stderr,
    )
    raise

setup(
    name="hamming_codec",
    version="0.1.0",
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
    package_dir={"":"src/python"},
    packages=["hamming_codec", "cli"],
    install_requires=["click"],
    cmake_args=["-DBUILD_PYTHON=on"],
    cmake_install_dir="src/python/hamming_codec",
    entry_points={"console_scripts": ["hamming=cli:cli.hamming"]},
)
