import argparse
from src.lib.utils import convert, convert_file


def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--cli", action="store_true", help="start in command-line mode")
    parser.add_argument("--strs", type=str, help="the strs you want to convert")
    parser.add_argument("-f", type=str, help="the header file path you want to convert")
    parser.add_argument("-o", type=str, help="the output file path")
    parser.add_argument("-n", type=str, help="the output file name")
    return parser.parse_args()


def run_cli_mode(args):
    if args.strs:
        print(convert(args.strs))
    elif args.f:
        input_path = args.f
        output_path = args.o or "."
        output_name = args.n or "xxx"
        convert_file(input_path, output_path, output_name)


if __name__ == "__main__":
    args = get_args()
    run_cli_mode(args)
