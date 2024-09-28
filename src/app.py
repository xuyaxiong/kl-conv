import tkinter as tk
from tkinter import scrolledtext, messagebox, filedialog
from src import VERSION, APP_NAME, TITLE
from src.lib.utils import strip_header_file, convert


def choose_file():
    file_path = filedialog.askopenfilename()
    with open(file_path, "r", encoding="utf8") as f:
        data = f.read()
        striped_header_file = strip_header_file(data)
        input_text_area.delete("1.0", tk.END)
        input_text_area.insert(tk.END, striped_header_file)


def submit():
    input_text = input_text_area.get("1.0", tk.END).strip()
    output_text_area.delete("1.0", tk.END)
    output_text_area.insert(tk.END, convert(input_text, checkbox_var.get()) + "\n")


def copy():
    output_text = output_text_area.get("1.0", tk.END).strip()
    root.clipboard_clear()
    root.clipboard_append(output_text)
    messagebox.showinfo("Tips", "Copied to clipboard!")


def show_about():
    ABOUT = f"version: {VERSION}\nauthor: xuyax\ncontact: xyxlindy@163.com\nrepository: https://github.com/xuyaxiong/kl-conv"
    messagebox.showinfo("About", ABOUT)


root = tk.Tk()
root.title(f"{TITLE}V{VERSION}")

# 设置窗口大小
root.geometry("1200x800")

# 创建菜单栏
menubar = tk.Menu(root)
root.config(menu=menubar)

# 创建帮助菜单
help_menu = tk.Menu(menubar, tearoff=0)
menubar.add_cascade(label="Help", menu=help_menu)

# 添加About菜单项
help_menu.add_command(label="About", command=show_about)

# 创建Frame放置输入输出区域
frame = tk.Frame(root)
frame.pack(fill=tk.BOTH, expand=True)

# 文本输入区域
input_text_area = scrolledtext.ScrolledText(frame, width=50, height=10)
input_text_area.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
input_text_area.pack_propagate(False)

# 文本输出区域
output_text_area = scrolledtext.ScrolledText(frame, width=50, height=10)
output_text_area.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
output_text_area.pack_propagate(False)

# 控制面板
control_frame = tk.Frame(root)
control_frame.pack()

# 选择头文件
choose_file_button = tk.Button(control_frame, text="选择头文件", command=choose_file)
choose_file_button.pack(side=tk.LEFT, padx=10)

# 是否输出注释部分
checkbox_var = tk.BooleanVar()
checkbox = tk.Checkbutton(control_frame, text="跳过注释", variable=checkbox_var)
checkbox.pack(side=tk.LEFT, padx=10)

# 提交按钮
submit_button = tk.Button(control_frame, text="转换", command=submit)
submit_button.pack(side=tk.LEFT, padx=10)

# 复制转换结果按钮
copy_button = tk.Button(control_frame, text="复制", command=copy)
copy_button.pack(side=tk.LEFT, padx=10)

root.mainloop()
