import tkinter as tk
from tkinter import scrolledtext
from src import VERSION
from src.lib.utils import convert


def submit():
    input_text = input_text_area.get("1.0", tk.END).strip()
    output_text_area.delete("1.0", tk.END)
    output_text_area.insert(tk.END, convert(input_text) + "\n")


root = tk.Tk()
root.title(f"考拉C++函数声明转换V{VERSION}(xuyax)")

# 设置窗口最大化
root.state("zoomed")

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

# 提交按钮
submit_button = tk.Button(root, text="转换", command=submit)
submit_button.pack(pady=10)

root.mainloop()
