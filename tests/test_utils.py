import pytest
from src.lib.utils import (
    convert,
    format_comm,
    parse_decl,
    split_docstring_to_comm_and_decl,
    split_to_docstring,
)
from src.lib.test_data import docstrings, docstring, decl, comm

SKIP = False


@pytest.mark.skipif(SKIP, reason="temporary skip")
def test_convert():
    print(convert(docstrings))


@pytest.mark.skipif(SKIP, reason="temporary skip")
def test_format_comm():
    print(format_comm(comm))


@pytest.mark.skipif(SKIP, reason="temporary skip")
def test_parse_decl():
    print(parse_decl(decl))


@pytest.mark.skipif(SKIP, reason="temporary skip")
def test_split_docstring_to_comm_and_decl():
    print(split_docstring_to_comm_and_decl(docstring))


@pytest.mark.skipif(SKIP, reason="temporary skip")
def test_split_to_docstring():
    print(split_to_docstring(docstrings))
