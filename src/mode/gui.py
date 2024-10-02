import tkinter as tk
from tkinter import scrolledtext, messagebox, filedialog
from src.lib.utils import get_file_content, strip_header_file, convert
from src import VERSION, APP_NAME, TITLE


class GUI:
    def __init__(self):
        self.file_path = ""
        self.init_ui()

    def init_ui(self):
        self.root = tk.Tk()
        self.root.title(f"{TITLE}V{VERSION}")

        # 设置窗口大小
        self.root.geometry("1200x800")

        # 创建菜单栏
        menubar = tk.Menu(self.root)
        self.root.config(menu=menubar)

        # 创建帮助菜单
        help_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="Help", menu=help_menu)

        # 添加About菜单项
        help_menu.add_command(label="About", command=self.show_about)

        # 创建Frame放置输入输出区域
        frame = tk.Frame(self.root)
        frame.pack(fill=tk.BOTH, expand=True)

        # 文本输入区域
        self.input_text_area = scrolledtext.ScrolledText(frame, width=50, height=10)
        self.input_text_area.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        self.input_text_area.pack_propagate(False)

        # 文本输出区域
        self.output_text_area = scrolledtext.ScrolledText(frame, width=50, height=10)
        self.output_text_area.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
        self.output_text_area.pack_propagate(False)

        # 控制面板
        control_frame = tk.Frame(self.root, padx=10, pady=10)
        control_frame.pack()

        # 选择头文件
        choose_file_button = tk.Button(
            control_frame, text="选择头文件", command=self.choose_file
        )
        choose_file_button.pack(side=tk.LEFT, padx=10)

        # 是否输出注释部分
        self.checkbox_var = tk.BooleanVar()
        checkbox = tk.Checkbutton(
            control_frame, text="跳过注释", variable=self.checkbox_var
        )
        checkbox.pack(side=tk.LEFT, padx=10)

        # 提交按钮
        submit_button = tk.Button(
            control_frame, text="转换", command=self.submit, bg="#3498db", fg="white"
        )
        submit_button.pack(side=tk.LEFT, padx=10)

        # 复制转换结果按钮
        copy_button = tk.Button(control_frame, text="复制", command=self.copy)
        copy_button.pack(side=tk.LEFT, padx=10)

    def choose_file(self):
        file_path = filedialog.askopenfilename(
            title="选择头文件",
            filetypes=[("Header Files", "*.h")],
        )
        if file_path == "":
            return
        data = get_file_content(file_path)
        striped_header_file = strip_header_file(data)
        self.input_text_area.delete("1.0", tk.END)
        self.input_text_area.insert(tk.END, striped_header_file)

    def submit(self):
        input_text = self.input_text_area.get("1.0", tk.END).strip()
        self.output_text_area.delete("1.0", tk.END)
        self.output_text_area.insert(
            tk.END, convert(input_text, self.checkbox_var.get()) + "\n"
        )

    def copy(self):
        output_text = self.output_text_area.get("1.0", tk.END).strip()
        self.root.clipboard_clear()
        self.root.clipboard_append(output_text)
        messagebox.showinfo("Tips", "Copied to clipboard!")

    def show_about(self):
        ABOUT = f"version: {VERSION}\nauthor: xuyax\ncontact: xyxlindy@163.com\nrepo: https://github.com/xuyaxiong/kl-conv"
        messagebox.showinfo("About", ABOUT)

    def start(self):
        self.root.mainloop()


def run_gui_mode():
    GUI().start()


if __name__ == "__main__":
    run_gui_mode()
