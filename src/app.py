import argparse
from src.mode.gui import run_gui_mode
from src.mode.cli import run_cli_mode
from src.mode.serve import run_serve_mode


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--cli", action="store_true", help="start in command-line mode")
    parser.add_argument(
        "--docstrings", type=str, help="the docstrings you want to convert"
    )
    parser.add_argument("-f", type=str, help="the header file path you want to convert")
    parser.add_argument("-o", type=str, help="the output file path")
    parser.add_argument("-n", type=str, help="the output file name")
    args = parser.parse_args()
    if args.cli:
        run_cli_mode(args)
    else:
        run_serve_mode()
        run_gui_mode()
