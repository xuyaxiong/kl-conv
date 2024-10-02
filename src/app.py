from src.mode.gui import run_gui_mode
from src.mode.cli import run_cli_mode, get_args
from src.mode.serve import run_serve_mode


if __name__ == "__main__":
    args = get_args()
    if args.cli:
        run_cli_mode(args)
    else:
        run_serve_mode()
        run_gui_mode()
