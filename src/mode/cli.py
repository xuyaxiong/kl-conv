from src.lib.utils import convert, convert_file


def run_cli_mode(args):
    if args.docstrings:
        print(convert(args.docstrings))
    elif args.f:
        input_path = args.f
        output_path = args.o or "."
        output_name = args.n or "xxx"
        convert_file(input_path, output_path, output_name)
