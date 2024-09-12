from src.lib.utils import (
    convert,
    parse_decl,
    split_docstring_to_comm_and_decl,
    split_to_docstring,
)
from src.lib.test_data import docstrings, docstring, decl


def test_convert():
    print(convert(docstrings))


def test_parse_decl():
    print(parse_decl(decl))


def test_split_docstring_to_comm_and_decl():
    print(split_docstring_to_comm_and_decl(docstring))


def test_split_to_docstring():
    print(split_to_docstring(docstrings))
