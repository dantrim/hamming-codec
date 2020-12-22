import pathlib
from setuptools import setup, find_packages

# the directory containing this file
HERE = pathlib.Path(__file__).parent

# the text of the README file
README = (HERE / "README.rst").read_text()

setup(
    name="hamming-codec",
    version="1.0.0",
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
    package_dir={"": "src"},
    packages=find_packages(where="src"),
    include_package_data=True,
    install_requires=["numpy", "click"],
    entry_points={"console_scripts": ["hamming=cli:cli.hamming"]},
)
