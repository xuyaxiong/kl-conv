import pytest
from src.lib.utils import (
    preprocess,
    convert,
    split_to_docstring_list,
    split_docstring_to_comm_and_decl,
    format_comm,
    parse_decl,
    export_comm_and_decl,
)
from src.lib.test_data import (
    docstrings,
    docstring,
    decl,
    comm,
    parsed_decl,
    formated_comm,
)

SKIP = False


@pytest.mark.skipif(SKIP, reason="temporary skip")
def test_preprocess():
    print(preprocess(docstrings))


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
    print(split_to_docstring_list(docstrings))


@pytest.mark.skipif(SKIP, reason="temporary skip")
def test_export_comm_and_decl():
    print(export_comm_and_decl(formated_comm, parsed_decl, False))
    print(export_comm_and_decl(formated_comm, parsed_decl, True))
