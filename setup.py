import subprocess


def build_executable():
    # PyInstaller 命令参数
    command = [
        "pyinstaller",  # 调用 pyinstaller
        "--onefile",  # 将所有文件打包成一个可执行文件
        "--noconsole",  # 关闭控制台窗口 (Windows)
        "--name=func_decl_converter",  # 指定生成的可执行文件名称
        "./src/app.py",  # 要打包的 Python 文件
    ]

    # 调用 subprocess.run 来执行命令
    try:
        result = subprocess.run(command, check=True)
        print("打包成功！")
    except subprocess.CalledProcessError as e:
        print(f"打包失败：{e}")


if __name__ == "__main__":
    build_executable()
