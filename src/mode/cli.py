import os
from src.lib.utils import get_file_content, strip_header_file, convert, fill_template


def run_cli_mode(args):
    if args.docstrings:
        print(convert(args.docstrings))
    elif args.f:
        input_path = args.f
        output_path = args.o or "."
        output_name = args.n or "xxx"
        data = get_file_content(input_path)
        striped_header_file = strip_header_file(data)
        content = convert(striped_header_file)
        res = fill_template(output_name, content)
        output_path = os.path.join(output_path, f"{output_name}.ts")
        with open(output_path, "w", encoding="utf-8") as f:
            f.write(res)
