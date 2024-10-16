import pytest
from src.lib.utils import (
    preprocess,
    convert,
    format_comm,
    parse_decl,
    export_comm_and_decl,
    fill_template,
)
from src.lib.test_data import (
    docstrings,
    docstring,
    decl,
    comm,
    parsed_decl,
    formatted_comm,
    formatted_docstring,
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
def test_export_comm_and_decl():
    print(export_comm_and_decl(formatted_comm, parsed_decl, False))
    print(export_comm_and_decl(formatted_comm, parsed_decl, True))


@pytest.mark.skipif(SKIP, reason="temporary skip")
def test_fill_template():
    print(fill_template("testDll", formatted_docstring))
