import subprocess
from pathlib import Path
from src import VERSION, APP_NAME


def remove_file(file_path):
    file_path = Path(file_path)
    try:
        file_path.unlink()
        print(f"Deleted: {file_path}")
    except FileNotFoundError:
        print(f"File not found: {file_path}")
    except PermissionError:
        print(f"Permission denied: {file_path}")
    except Exception as e:
        print(f"Error occurred: {e}")


def build_executable(is_gui=True):
    output_name = f"{APP_NAME}_{'gui' if is_gui else 'cli'}_{VERSION}"
    # PyInstaller 命令参数
    command = [
        "pyinstaller",  # 调用 pyinstaller
        "--onefile",  # 将所有文件打包成一个可执行文件
        "--windowed" if is_gui else "--console",
        "--add-data=assets:assets",
        f"--name={output_name}",  # 指定生成的可执行文件名称
        "./src/app.py",  # 要打包的 Python 文件
    ]

    # 调用 subprocess.run 来执行命令
    try:
        result = subprocess.run(command, check=True)
        remove_file(f"{output_name}.spec")
        print(f"{output_name}打包成功！")
    except subprocess.CalledProcessError as e:
        print(f"{output_name}打包失败：{e}")


if __name__ == "__main__":
    build_executable()
    build_executable(False)
