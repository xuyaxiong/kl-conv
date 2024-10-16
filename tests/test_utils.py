import pytest
from src.lib.utils import (
    convert,
    parse_decl,
    fill_template,
)
from src.lib.test_data import (
    docstrings,
    decl,
    formatted_docstring,
)

SKIP = False


@pytest.mark.skipif(SKIP, reason="temporary skip")
def test_convert():
    print(convert(docstrings))


@pytest.mark.skipif(SKIP, reason="temporary skip")
def test_parse_decl():
    print(parse_decl(decl))


@pytest.mark.skipif(SKIP, reason="temporary skip")
def test_fill_template():
    print(fill_template("testDll", formatted_docstring))
